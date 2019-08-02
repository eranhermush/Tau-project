#ifndef TAU_PROJECT_KDF_SAE_H
#define TAU_PROJECT_KDF_SAE_H

#include <string>

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
		KDF_SAE() =default;

		/* Constructor */
		KDF_SAE(const HMAC& hmac_object): hmac(hmac_object) {}

		/* Copy Constructor */
		KDF_SAE(const KDF_SAE<HMAC>& kdf): hmac(kdf.hmac) {}

		/* Key derivation function
			Returns a key of bit_length bits (bit_length < 2^16)
			is bit_length is not multiple of 8, the remaining bits in the last byte are 0 */
		std::string derive_key(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length);

	private:
		HMAC hmac;
};

template <class HMAC>
std::string KDF_SAE<HMAC>::derive_key(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length){
	// byte_length = ceiling(bit_length / 8)
	unsigned int byte_length = (bit_length + 7) / 8;
	unsigned int digest_size = hmac.DigestSize();
	// #interations = ceiling(bit_length / digest_bit_length)
	unsigned int iterations = (bit_length + digest_size * sizeof(char) * 8 - 1) / (digest_size * sizeof(char) * 8);
	std::string derived;
	std::string digest;
	digest.resize(digest_size);

	hmac.SetKey((const unsigned char*) kdf_key.data(), kdf_key.size());

	std::string message_suffix(label);
	message_suffix.append(context, 0, context.size());
	// push bit_length as a Little-Endian 2-byte short 
	message_suffix.push_back(bit_length % 256);
	message_suffix.push_back((bit_length / 256) % 256);

	unsigned int message_size = 2 + message_suffix.size();
	for(unsigned int i = 1; i <= iterations; ++i){
		// derived <- derived || HMAC(kdf_key, i || label || context || bit_length)
		std::string message;
		message.reserve(message_size);
		// push bit_length as a Little-Endian 2-byte shor
		message.push_back(i % 256);
		message.push_back((i / 256) % 256);
		message.append(message_suffix, 0, message_suffix.size());

		hmac.CalculateDigest((unsigned char*) &digest[0], (const unsigned char*) message.data(), message.size());

		derived.append(digest, 0, digest_size);
	}
	// return the only the first bit_length bits
	// as specified in IEEE Std 802.11-2012 11.6.1.7.2
	derived = derived.substr(0, byte_length);
	if(bit_length % 8 != 0){
		// the derived key is viewed as a Big-Endian,
		// therefore, if the bit_length % 8 != 0
		//			then bitwise-right-shift the string by 8 - bit_length % 8 (with 0 extend)
		unsigned char temp = 0;
		unsigned char last_char = 0;
		for(unsigned int i = 0; i < derived.size(); ++i){
			temp = (unsigned char) derived[i];
			last_char <<= (bit_length % 8);
			derived[i] = (temp >> (8 - (bit_length % 8))) | last_char;
			last_char = temp;
		}
	}
	return derived;
}

#endif