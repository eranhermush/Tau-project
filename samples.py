# Some quick and dirty code for WPA3-SAE operations
import struct, math, random
import sys
from Crypto.Hash import HMAC, SHA256, SHA384
from Crypto.PublicKey import ECC
from Crypto.Math.Numbers import Integer


# ----------------------- Elliptic Curve Operations ---------------------------------

# group 19
grp19_p = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
grp19_a = grp19_p - 3
grp19_b = 0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b

#group 21
grp21_p = 6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151
grp21_a = grp21_p - 3
grp21_b = 1093849038073734274511112390766805569936207598951683748994586394495953116150735016013708737573759623248592132296706313309438452531591012912142327488478985984

#group 22
grp22_p = 124325339146889384540494091085456630009856882741872806181731279018491820800119460022367403769795008250021191767583423221479185609066059226301250167164084041279837566626881119772675984258163062926954046545485368458404445166682380071370274810671501916789361956272226105723317679562001235501455748016154805420913
grp22_q = 1399252811935680595399801714158014275474696840019

#group 24
grp24_p = 17125458317614137930196041979257577826408832324037508573393292981642667139747621778802438775238728592968344613589379932348475613503476932163166973813218698343816463289144185362912602522540494983090531497232965829536524507269848825658311420299335922295709743267508322525966773950394919257576842038771632742044142471053509850123605883815857162666917775193496157372656195558305727009891276006514000409365877218171388319923896309377791762590614311849642961380224851940460421710449368927252974870395873936387909672274883295377481008150475878590270591798350563488168080923804611822387520198054002990623911454389104774092183
grp24_q = 63762351364972653564641699529205510489263266834182771617563631363277932854227

def legendre_symbol(a, p):
    """ Compute the Legendre symbol a|p using Euler's criterion. p is a prime, a is
        relatively prime to p (if p divides a, then a|p = 0)

        Returns 1 if a has a square root modulo p, -1 otherwise.
    """
    ls = pow(a, (p - 1) / 2, p)
    return -1 if ls == p - 1 else ls

def modular_sqrt(a, p):
    """See https://eli.thegreenplace.net/2009/03/07/computing-modular-square-roots-in-python"""
    if legendre_symbol(a, p) != 1:
        # print("modular_sqrt: no square root")
        return 0
    elif a == 0:
        # print("modular_sqrt: a == 0")
        return 0
    elif p == 2:
        # print("modular_sqrt: p == 2")
        return 0
    elif p % 4 == 3:
        return pow(a, (p + 1) / 4, p)

    s = p - 1
    e = 0
    while s % 2 == 0:
        s /= 2
        e += 1

    n = 2
    while legendre_symbol(n, p) != -1:
        n += 1

    x = pow(a, (s + 1) / 2, p)
    b = pow(a, s, p)
    g = pow(n, s, p)
    r = e

    while True:
        t = b
        m = 0
        for m in xrange(r):
            if t == 1:
                break
            t = pow(t, 2, p)

        if m == 0:
            return x

        gs = pow(g, 2 ** (r - m - 1), p)
        g = (gs * gs) % p
        x = (x * gs) % p
        b = (b * g) % p
        r = m

def point_on_curve(point):
	rhs = (point.x ** 3 + Integer(secp256r1_a) * point.x + Integer(secp256r1_b)) % Integer(secp256r1_p)
	lhs = (point.y ** 2) % Integer(secp256r1_p)
	return lhs == rhs

def int_to_data(num):
	return ("%064x" % int(num)).decode("hex")

def point_to_data(p):
	if p is None:
		return zeropoint_to_data()
	return int_to_data(p.x) + int_to_data(p.y)

def zeropoint_to_data():
	return int_to_data(0) + int_to_data(0)

def bit_shift_right(s, shift):
	last_chr = 0
	tmp = 0
	res = ""
	for ch in s:
		tmp = ord(ch)
		last_chr <<= (8 - shift)
		last_chr &= 0xff
		res += chr(tmp >> shift | last_chr)
		last_chr = tmp
	return res


# ----------------------- SAE/Dragonfly functions ---------------------------------

def HMAC256(pw, data):
	h = HMAC.new(pw, digestmod=SHA256)
	h.update(data)
	return h.digest()

def HMAC384(pw, data):
	h = HMAC.new(pw, digestmod=SHA384)
	h.update(data)
	return h.digest()

def KDF_Length256(data, label, context, length):
	iterations = int(math.ceil(length / 256.0))
	result = ""
	for i in range(1, iterations + 1):
		result += HMAC256(data, struct.pack("<H", i) + label + context + struct.pack("<H", length))
	result = result[:(length + 7) / 8]
	if length % 8 != 0:
		result = bit_shift_right(result, 8 - length % 8)
	return result


def KDF_Length384(data, label, context, length):
	iterations = int(math.ceil(length / 384.0))
	result = ""
	for i in range(1, iterations + 1):
		result += HMAC384(data, struct.pack("<H", i) + label + context + struct.pack("<H", length))
	result = result[:(length + 7) / 8]
	if length % 8 != 0:
		result = bit_shift_right(result, 8 - length % 8)
	return result


def sample_pwe_ecc(counter, password, addr1, addr2, p, a, b, hmac, kdf):
	addr1 = addr1.replace(':', '').decode("hex")
	addr2 = addr2.replace(':', '').decode("hex")
	hash_pw = addr1 + addr2 if addr1 > addr2 else addr2 + addr1
	p_string = ("%x" % p)
	if len(p_string) % 2:
		p_string = "0" + p_string 
	p_data = p_string.decode("hex")
	bits = int(math.ceil(math.log(p, 2)))
	hash_data = password + struct.pack("<B", counter)
	if hmac == 256:
		pwd_seed = HMAC256(hash_pw, hash_data)
	else:
		pwd_seed = HMAC384(hash_pw, hash_data)
	if kdf == 256:
		pwd_value = KDF_Length256(pwd_seed, "SAE Hunting and Pecking", p_data, bits)
	else:
		pwd_value = KDF_Length384(pwd_seed, "SAE Hunting and Pecking", p_data, bits)
	pwd_value = int(pwd_value.encode("hex"), 16)
	
	if pwd_value >= p:
		return False
	y_sqr = (pwd_value**3 + a * pwd_value + b) % p
	return legendre_symbol(y_sqr, p) == 1


def sample_pwe_ffc(counter, password, addr1, addr2, p, q, hmac, kdf):
	addr1 = addr1.replace(':', '').decode("hex")
	addr2 = addr2.replace(':', '').decode("hex")
	hash_pw = addr1 + addr2 if addr1 > addr2 else addr2 + addr1
	p_string = ("%x" % p)
	if len(p_string) % 2:
		p_string = "0" + p_string 
	p_data = p_string.decode("hex")
	bits = int(math.ceil(math.log(p, 2)))
	hash_data = password + struct.pack("<B", counter)
	if hmac == 256:
		pwd_seed = HMAC256(hash_pw, hash_data)
	else:
		pwd_seed = HMAC384(hash_pw, hash_data)
	if kdf == 256:
		pwd_value = KDF_Length256(pwd_seed, "SAE Hunting and Pecking", p_data, bits)
	else:
		pwd_value = KDF_Length384(pwd_seed, "SAE Hunting and Pecking", p_data, bits)
	pwd_value = int(pwd_value.encode("hex"), 16)
	
	if pwd_value >= p:
		return False
	pwe = pow(pwd_value, (p-1)/q, p)
	if pwe == 1:
		print "weird"
		return False
	return True



def get_ecc_sample_data(password, target_mac, p, a, b, hmac=256, kdf=256):
	print password
	print "HMAC %d" % hmac, "KDF %d" % kdf
	if p == grp19_p:
		print "group19"
	elif p == grp21_p:
		print "group21"
	prefix = "65:61:02:56:"
	count = 0
	while count < 40:
		mac = prefix + struct.pack("<B", random.randint(0,255)).encode("hex") + ':' + struct.pack("<B", random.randint(0,255)).encode("hex")
		for counter in range(1,256):
			val = sample_pwe_ecc(counter, password, mac, target_mac, p, a, b, hmac, kdf)
			print mac, target_mac, counter, int(val)
			count += 1
			if val:
				break


def get_ffc_sample_data(password, target_mac, p, q, hmac=256, kdf=256):
	print password
	print "HMAC %d" % hmac, "KDF %d" % kdf
	if p == grp22_p:
		print "group22"
	elif p == grp24_p:
		print "group24"
	prefix = "65:61:02:56:"
	count = 0
	while count < 40:
		mac = prefix + struct.pack("<B", random.randint(0,255)).encode("hex") + ':' + struct.pack("<B", random.randint(0,255)).encode("hex")
		for counter in range(1,256):
			val = sample_pwe_ffc(counter, password, mac, target_mac, p, q, hmac, kdf)
			print mac, target_mac, counter, int(val)
			count += 1
			if val:
				break


if __name__ == "__main__":
	if len(sys.argv) > 3:
		if sys.argv[1] == 'ecc' or sys.argv[1] == 'ECC':
			if(sys.argv[2] == '19'):
				get_ecc_sample_data(sys.argv[3], sys.argv[4], grp19_p, grp19_a, grp19_b)
			elif(sys.argv[2] == '21'):
				get_ecc_sample_data(sys.argv[3], sys.argv[4], grp21_p, grp21_a, grp21_b)
		elif sys.argv[1] == 'ffc' or sys.argv[1] == 'FFC':
			if(sys.argv[2] == '22'):
				get_ffc_sample_data(sys.argv[3], sys.argv[4], grp22_p, grp22_q)
			elif(sys.argv[2] == '24'):
				get_ffc_sample_data(sys.argv[3], sys.argv[4], grp24_p, grp24_q)
	else:
		print "Invalid use. Expecting 4 arguments:\n\ttype (ecc of ffc)\n\tgroup name (19, 21, 22, 24 are supported)\n\tpassword\n\tvictim mac"
