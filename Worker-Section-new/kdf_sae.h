#ifndef TAU_PROJECT_KDF_SAE_H
#define TAU_PROJECT_KDF_SAE_H

#include <string>
#include <climits>

/* 
	An implementation of the KDF used in the WPA3-SAE to be used in
	the template of PWE_Sample_Matcher (defined in pwe_sample_matcher.h)

	The template parameter HMAC represents a HMAC function object, namely it should implement
	 1) The function: unsigned int DigestSize()
	 	(expected to return the hash digest size in bytes)
	 2) The function: void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t input_length)
		(expected to calculate the HMAC of input and write the result in digest)
	 3) The function: void SetKey(const unsigned char* key, size_t key_length)
	 	(expected to update the key used in the HMAC)
	 4) A default constructor and a copy constructor
	
	For example HMAC can be a Crypto++ template class of the form HMAC<Hash>
*/

template <class HMAC>
class KDF_SAE{
	public:
		/* Default Constructor */
		KDF_SAE(){
			digest_buffer.resize(hmac.DigestSize());
		}

		/* Constructor */
		KDF_SAE(const HMAC& hmac_object): hmac(hmac_object) {
			digest_buffer.resize(hmac.DigestSize());
		}

		/* Copy Constructor */
		KDF_SAE(const KDF_SAE<HMAC>& kdf): hmac(kdf.hmac), digest_buffer(kdf.digest_buffer) {}

		/* Key derivation function in IEEE Std 802.11-2012 11.6.1.7.2
			Returns a key of bit_length bits (bit_length < 2^16)

			If bit_length is not multiple of 8, the leading bits in the first byte are 0 */
		std::string derive_key(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length);

		/* Key derivation function in IEEE Std 802.11-2012 11.6.1.7.2
			Place a key of bit_length bits (bit_length < 2^16) in result_buffer
			result_buffer should be large enough to contain bit_length_bits

			If bit_length is not multiple of 8, the leading bits in the first byte are 0 */
		void derive_key_to_buffer(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length,
			unsigned char* result_buffer);

	private:
		HMAC hmac;
		std::string digest_buffer;
};

template <class HMAC>
std::string KDF_SAE<HMAC>::derive_key(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length){
	std::string derived;
	unsigned int result_length = (bit_length + CHAR_BIT - 1) / CHAR_BIT;
	derived.resize(result_length, 0);
	derive_key_to_buffer(kdf_key, label, context, bit_length, (unsigned char*) &derived[0]);
	return derived;
}



template <class HMAC>
void KDF_SAE<HMAC>::derive_key_to_buffer(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length, unsigned char* result_buffer){
	// result_length = ceiling(bit_length / sizeof char in bits)
	unsigned int result_length = (bit_length + CHAR_BIT - 1) / CHAR_BIT;
	unsigned int digest_size = hmac.DigestSize(); // in bytes
	// #interations = ceiling(bit_length / digest_bit_length)
	unsigned int iterations = (bit_length + digest_size * CHAR_BIT - 1) / (digest_size * CHAR_BIT);

	hmac.SetKey((const unsigned char*) kdf_key.data(), kdf_key.size());

	std::string message;
	message.reserve(2 + label.size() + context.size() + 2);
	message.append(2, 0);
	message.append(label, 0, label.size());
	message.append(context, 0, context.size());
	// push bit_length as a Little-Endian 2-byte short 
	message.push_back(bit_length % 256);
	message.push_back((bit_length / 256) % 256);

	unsigned int result_offset = 0;
	for(unsigned int i = 1; i <= iterations; ++i){
		// derived <- derived || HMAC(kdf_key, i || label || context || bit_length)
		// first 2 bits are bit_length as a Little-Endian 2-byte shor
		message[0] = (i % 256);
		message[1] = ((i / 256) % 256);
		
		unsigned char* digest_start = (unsigned char*) &digest_buffer[0];
		hmac.CalculateDigest(digest_start, (const unsigned char*) message.data(), message.size());

		unsigned int copy_length = ((result_length - result_offset) > digest_size) ? digest_size : (result_length - result_offset);
		std::copy(digest_start, digest_start + copy_length, result_buffer + result_offset);

		result_offset += copy_length;
	}
	// return the only the first bit_length bits
	// as specified in IEEE Std 802.11-2012 11.6.1.7.2
	if(bit_length % CHAR_BIT != 0){
		// the derived key is viewed as a Big-Endian,
		// therefore, if there are extra bits
		//			then bitwise-right-shift the string by 8 - bit_length % 8 (with 0 extend)
		unsigned int extra = bit_length % CHAR_BIT;
		unsigned int shift = CHAR_BIT - extra;
		unsigned char temp = 0;
		unsigned char last_char = 0;
		for(unsigned int i = 0; i < result_length; ++i){
			temp = result_buffer[i];
			last_char <<= extra;
			result_buffer[i] = (temp >> shift) | last_char;
			last_char = temp;
		}
	}
}




#endif