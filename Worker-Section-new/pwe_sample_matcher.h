#ifndef TAU_PROJECT_PWE_SAMPLE_MATCHER_H
#define TAU_PROJECT_PWE_SAMPLE_MATCHER_H

#include "preimage_matcher.h"
#include "pwe_sample.h"
#include <string>
#include <vector>
#include <algorithm>
#include <NTL/ZZ.h>

/*
	PWE_Sample_Matcher is a Preimage_Matcher which tries to match its inputs to a series
	of samples about password-to-element function that was run on it.
	It is assumed that the password-to-elemnet function takes as inputs (in addition to a
	password) two strings (representing MAC addresses), and a counter which is a byte.
	The result of a sample is assumed to be represented by a char.


	The template parameter HMAC represents a HMAC function object used to create the seed for
	the KDF, namely it should implement
	 1) The function: unsigned int DigestSize()
	 	(expected to return the hash digest size in bytes)
	 2) The function: void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t input_length)
		(expected to calculate the HMAC of input and write the result in digest)
	 3) The function: void SetKey(const unsigned char* key, size_t key_length)
	 	(expected to update the key used in the HMAC)
	
	For example HMAC can be a Crypto++ template class of the form HMAC<Hash>


	The template parameter KDF represents a key derivation function object in the form used in WPA3-SAE,
	it should implement:
	 1) The function std::string derive_key(const std::string& kdf_key, const std::string& label, const std::string& context, unsigned int bit_length)
	 	(expected to calculate and return the derived key)

	Notice that this is a different interface to the KDF in Crytpo++, as even the abstract interface is not the same.
	However, one can wrap a Crypto++ KDF object and handle the adjustments (mainly, varying derived key length)
*/


template <class KDF ,class HMAC>
class PWE_Sample_Matcher: public Preimage_Matcher{
	public:
		virtual ~PWE_Sample_Matcher() =default;

		/* Constructor 
			Uses a samples vector */
		PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj, const HMAC& hmac_obj);

		/* Constructor 
			Uses a samples parameters */
		PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs, const std::vector<std::string>& target_macs,
			const std::vector<unsigned char>& counters, const std::vector<char>& results, const KDF& kdf_obj, const HMAC& hmac_obj);

		static NTL::ZZ derive_element(KDF& kdf_obj, HMAC& hmac_obj, const NTL::ZZ& prime, const std::string& password, const pwe_sample& sample);

		/* tests a password with a single sample */
		virtual bool simulate_test(const std::string& password, const pwe_sample& sample) =0;

		/* tests a password with all samples */
		virtual bool test(const std::string& input);

		static const std::string SAE_label;

	protected:
		std::vector<pwe_sample> samples;
		KDF kdf;
		HMAC hmac;
};

template <class KDF ,class HMAC>
const std::string PWE_Sample_Matcher<KDF ,HMAC>::SAE_label = "SAE Hunting and Pecking";

template <class KDF ,class HMAC>
PWE_Sample_Matcher<KDF ,HMAC>::PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj, const HMAC& hmac_obj)
	: samples(samps), kdf(kdf_obj), hmac(hmac_obj) {}


template <class KDF ,class HMAC>
PWE_Sample_Matcher<KDF ,HMAC>::PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs, const std::vector<std::string>& target_macs,
	const std::vector<unsigned char>& counters, const std::vector<char>& results, const KDF& kdf_obj, const HMAC& hmac_obj)
	: samples(pwe_sample::create_sample_vector(spoofed_macs, target_macs, counters, results)), kdf(kdf_obj), hmac(hmac_obj) {}


template <class KDF ,class HMAC>
NTL::ZZ PWE_Sample_Matcher<KDF ,HMAC>::derive_element(KDF& kdf_obj, HMAC& hmac_obj, const NTL::ZZ& prime,
	const std::string& password, const pwe_sample& sample){

	// calculate HMAC key from MAC address [max||min]
	unsigned char hmac_key[12];
	if(std::lexicographical_compare(sample.mac1, sample.mac1 + 6, sample.mac2, sample.mac2 + 6)){
		// mac1 < mac2 => hmac_key = mac2 + mac1
		std::copy(sample.mac2, sample.mac2 + 6, hmac_key);
		std::copy(sample.mac1, sample.mac1 + 6, hmac_key + 6);
	}
	else{
		std::copy(sample.mac1, sample.mac1 + 6, hmac_key);
		std::copy(sample.mac2, sample.mac2 + 6, hmac_key + 6);
	}
	hmac_obj.SetKey(hmac_key, sizeof(hmac_key));

	// Calculate KDF key by HMAC-ing the password and counter [HMAC(hmac_key, password||counter)]
	std::string hmac_message(password);
	hmac_message.push_back(sample.counter);
	std::string kdf_key;
	kdf_key.resize(hmac_obj.DigestSize());
	hmac_obj.CalculateDigest((unsigned char*) &kdf_key[0], (const unsigned char*) hmac_message.data(), hmac_message.size());

	// Derive bytes-key from the kdf_key, the label and the prime [KDF(kdf_key, label, BigEndianBytes(prime), BitLength(prime))]
	unsigned int bits = (unsigned int) NTL::NumBits(prime); // for all normal uses, bits should be a multiple of 8
	unsigned int bytes = (unsigned int) NTL::NumBytes(prime);
	std::string prime_bytes;
	prime_bytes.resize(bytes);
	NTL::BytesFromZZ((unsigned char*) &prime_bytes[0], prime, bytes);
	// NTL encodes as Little-Endian, but the algorithm encodes the prime as Big-Endian
	std::reverse(prime_bytes.begin(), prime_bytes.end());
	std::string derived(kdf_obj.derive_key(kdf_key, PWE_Sample_Matcher<KDF ,HMAC>::SAE_label, prime_bytes, bits));

	// Derive a big integer from the bytes-key [BigEndianNumber(derived)]
	// NTL decodes bytes as Little-Endian, but the algorithm decodes the derived bytes as Big-Endian
	std::reverse(derived.begin(), derived.end());
	return NTL::ZZFromBytes((const unsigned char*) derived.data(), derived.size());

}


template <class KDF ,class HMAC>
bool PWE_Sample_Matcher<KDF, HMAC>::test(const std::string& input){
	for(unsigned int i = 0; i < samples.size(); ++i){
		if(!simulate_test(input, samples[i])){
			return false;
		}
	}
	return true;
}


#endif