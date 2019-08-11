#include "preimage_matcher_by_name.h"

// Converts a hexstring to a string of bytes
std::string hexstring_to_bytes(const std::string& hexstr);


std::unique_ptr<Preimage_Matcher> Matcher_By_Name::create_matcher(const std::string& name, const std::vector<std::string>& args){
	std::string lc_name(name);
	transform(lc_name.begin(), lc_name.end(), lc_name.begin(), [](unsigned char c){return std::tolower(c);});

	if(lc_name.find("sae") == 0){
		return Matcher_By_Name::create_sae_sample_matcher(lc_name, args);
	}
	// We now expect a hash_matcher
	// the first (and only argument) should be the target encoded as a hex string
	if(args.size() < 1){
		return nullptr;
	}
	std::string target(hexstring_to_bytes(args[0]));
	// SHA2-s
	if(lc_name.compare("sha2") == 0 || lc_name.compare("sha2-256") == 0 || lc_name.compare("sha256") == 0){
		CryptoPP::SHA256 sha256;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA256>(sha256, target));
	}
	if(lc_name.compare("sha384") == 0 || lc_name.compare("sha2-384") == 0){
		CryptoPP::SHA384 sha384;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA384>(sha384, target));
	}
	if(lc_name.compare("sha512") == 0 || lc_name.compare("sha2-512") == 0){
		CryptoPP::SHA512 sha512;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA512>(sha512, target));
	}
	// SHA3-s
	if(lc_name.compare("sha3") == 0 || lc_name.compare("sha3-256") == 0){
		CryptoPP::SHA3_256 sha3256;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA3_256>(sha3256, target));
	}
	if(lc_name.compare("sha3-384") == 0){
		CryptoPP::SHA3_384 sha3384;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA3_384>(sha3384, target));
	}
	if(lc_name.compare("sha3-512") == 0){
		CryptoPP::SHA3_512 sha3512;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA3_512>(sha3512, target));
	}
	//SHA1
	if(lc_name.compare("sha1") == 0){
		CryptoPP::SHA1 sha1;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::SHA1>(sha1, target));
	}
	//MD5
	if(lc_name.compare("md5") == 0){
		CryptoPP::Weak::MD5 md5;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::Weak::MD5>(md5, target));
	}
	//Whirlpool
	if(lc_name.compare("whirlpool") == 0){
		CryptoPP::Whirlpool whirlpool;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::Whirlpool>(whirlpool, target));
	}
	//Keccak
	if(lc_name.compare("keccak") == 0 || lc_name.compare("keccak-256")){
		CryptoPP::Keccak_256 keccak;
		return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<CryptoPP::Keccak_256>(keccak, target));
	}
	return nullptr;
}



std::unique_ptr<Preimage_Matcher> Matcher_By_Name::create_sae_sample_matcher(const std::string& lc_name, const std::vector<std::string>& args){
	bool ecc;
	if(args.size() < 2){
		// improper arguments
		return nullptr;
	}
	if(lc_name.compare("sae ecc") == 0){
		ecc = true;
	}
	else if(lc_name.compare("sae ffc") == 0){
		ecc = false;
	}
	else{
		return nullptr;
	}
	std::vector<pwe_sample> samples(pwe_sample::create_sample_vector_from_file(args[0]));
	if(samples.size() < 1){
		// samples could not be extracted from file
		return nullptr;
	}
	// by group name
	CryptoPP::HMAC<CryptoPP::SHA256> hmac;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>> kdf(hmac);
	if(ecc){
		if(args[1].compare("group19") == 0){
			return std::unique_ptr<Preimage_Matcher>(
				new ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>>(samples, kdf, hmac, ECC::GROUP_NAMES::GROUP19));
		}
		else if(args[1].compare("group20") == 0){
			return std::unique_ptr<Preimage_Matcher>(
				new ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>>(samples, kdf, hmac, ECC::GROUP_NAMES::GROUP20));
		}
		else if(args[1].compare("group21") == 0){
			return std::unique_ptr<Preimage_Matcher>(
				new ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>>(samples, kdf, hmac, ECC::GROUP_NAMES::GROUP21));
		}
		// invalid group by
		return nullptr;
	}
	if(args[1].compare("group22") == 0){
		return std::unique_ptr<Preimage_Matcher>(
			new FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>>(samples, kdf, hmac, FFC::GROUP_NAMES::GROUP22));
	}
	else if(args[1].compare("group23") == 0){
		return std::unique_ptr<Preimage_Matcher>(
			new FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>>(samples, kdf, hmac, FFC::GROUP_NAMES::GROUP23));
	}
	else if(args[1].compare("group24") == 0){
		return std::unique_ptr<Preimage_Matcher>(
			new FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>>(samples, kdf, hmac, FFC::GROUP_NAMES::GROUP24));
	}
	// invalid group by
	return nullptr;
}








std::string hexstring_to_bytes(const std::string& hexstr){
	std::string result;
	result.reserve(hexstr.length() / 2);
	unsigned char temp_val;
	char temp_char;
	for(unsigned int i = 0; i < hexstr.length(); i += 2){
		temp_val = 0;
		temp_char = hexstr[i];
		if('a' <= temp_char && temp_char <= 'f'){
			temp_val += temp_char - 'a' + 10;
		}
		else if('A' <= temp_char && temp_char <= 'F'){
			temp_val += temp_char - 'A' +10;
		}
		else if('0' <= temp_char && temp_char <= '9'){
			temp_val += temp_char - '0';
		}
		if(i + 1 < hexstr.length()){
			temp_val *= 16;
			temp_char = hexstr[i+1];
			if('a' <= temp_char && temp_char <= 'f'){
				temp_val += temp_char - 'a' + 10;
			}
			else if('A' <= temp_char && temp_char <= 'F'){
				temp_val += temp_char - 'A' +10;
			}
			else if('0' <= temp_char && temp_char <= '9'){
				temp_val += temp_char - '0';
			}
		}
		result.push_back(temp_val);
	}
	return result;
}