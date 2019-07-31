#ifndef TAU_PROJECT_ECC_PWE_SAMPLE_MATCHER_H
#define TAU_PROJECT_ECC_PWE_SAMPLE_MATCHER_H

#include "sae_groups.h"
#include "pwe_sample_matcher.h"
#include <NTL/ZZ.h>

/*
	ECC_PWE_Sample_Matcher is PWE_Sample_Matcher with password-to-element function
	which maps passwords to elements of an elliptic curve modulo a prime.

	The same requirements of the KDF and HMAC parameters hold.
*/

/* 
	For a sample of ECC_PWE, (bool) sample.result <==> (the derived element x is valid)
	Which means 1) x is smaller than the prime modulus p
				2) x^3 + ax + b is a quadratic residue modulo p

	If x is uniformly sampled from (Z/pZ)*, then Pr[x is a QR] = 0.5.
*/

template <class KDF ,class HMAC>
class ECC_PWE_Sample_Matcher: public PWE_Sample_Matcher<KDF, HMAC>{
	public:
		/* Constructor
			Uses a sample vector and group paramters */
		ECC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj,
			const HMAC& hmac_obj, const NTL::ZZ& prime, const NTL::ZZ& a_coeff, const NTL::ZZ& b_coeff);

		/* Constructor
			Uses a sample vector and group name */
		ECC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps,  const KDF& kdf_obj,
			const HMAC& hmac_obj, ECC::GROUP_NAMES name);

		/* Constructor
			Uses a samples parameters and group paramters */
		ECC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs, const std::vector<std::string>& target_macs,
			const std::vector<unsigned char>& counters, const std::vector<char>& results,  const KDF& kdf_obj,
			const HMAC& hmac_obj, const NTL::ZZ& prime,  const NTL::ZZ& a_coeff, const NTL::ZZ& b_coeff);

		/* Constructor
			Uses a samples parameters and group name */
		ECC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs, const std::vector<std::string>& target_macs,
			const std::vector<unsigned char>& counters, const std::vector<char>& results,  const KDF& kdf_obj,
			const HMAC& hmac_obj, ECC::GROUP_NAMES name);


		/* Copy constructor */
		ECC_PWE_Sample_Matcher(const ECC_PWE_Sample_Matcher& ecc_matcher);

		std::unique_ptr<Preimage_Matcher> clone() const;

		/* tests a password with a single sample */
		bool simulate_test(const std::string& password, const pwe_sample& sample);


	protected:
		KDF kdf;
		HMAC hmac;
		NTL::ZZ p;
		NTL::ZZ a;
		NTL::ZZ b;

};


/*		Implementation		*/

/* Constructors */

template <class KDF ,class HMAC>
ECC_PWE_Sample_Matcher<KDF, HMAC>::ECC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj,
	const HMAC& hmac_obj, const NTL::ZZ& prime, const NTL::ZZ& a_coeff, const NTL::ZZ& b_coeff)
	: PWE_Sample_Matcher<KDF, HMAC>(samps, kdf_obj, hmac_obj), p(prime), a(a_coeff), b(b_coeff) {} 


template <class KDF ,class HMAC>
ECC_PWE_Sample_Matcher<KDF, HMAC>::ECC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps,  const KDF& kdf_obj,
	const HMAC& hmac_obj, ECC::GROUP_NAMES name)
	: ECC_PWE_Sample_Matcher<KDF, HMAC>(samps, kdf_obj, hmac_obj, ECC::GROUPS[(int) name].p,
			ECC::GROUPS[(int) name].a,  ECC::GROUPS[(int) name].b) {}


template <class KDF ,class HMAC>
ECC_PWE_Sample_Matcher<KDF, HMAC>::ECC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs,
	const std::vector<std::string>& target_macs, const std::vector<unsigned char>& counters, const std::vector<char>& results,
	const KDF& kdf_obj,	const HMAC& hmac_obj, const NTL::ZZ& prime,  const NTL::ZZ& a_coeff, const NTL::ZZ& b_coeff)
	: PWE_Sample_Matcher<KDF, HMAC>(spoofed_macs, target_macs, counters, results, kdf_obj, hmac_obj),
	p(prime), a(a_coeff), b(b_coeff) {}


template <class KDF ,class HMAC>
ECC_PWE_Sample_Matcher<KDF, HMAC>::ECC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs,
	const std::vector<std::string>& target_macs, const std::vector<unsigned char>& counters, const std::vector<char>& results,
	const KDF& kdf_obj,	const HMAC& hmac_obj, ECC::GROUP_NAMES name)
	: ECC_PWE_Sample_Matcher<KDF, HMAC>(spoofed_macs, target_macs, counters, results, kdf_obj, hmac_obj,
			ECC::GROUPS[(int) name].p, ECC::GROUPS[(int) name].a,  ECC::GROUPS[(int) name].b) {}


template <class KDF ,class HMAC>
ECC_PWE_Sample_Matcher<KDF, HMAC>::ECC_PWE_Sample_Matcher(const ECC_PWE_Sample_Matcher& ecc_matcher)
	: PWE_Sample_Matcher<KDF, HMAC>(ecc_matcher.samples, ecc_matcher.kdf, ecc_matcher.hmac),
	p(ecc_matcher.p), a(ecc_matcher.a), b(ecc_matcher.b) {}

/* */

template <class KDF ,class HMAC>
std::unique_ptr<Preimage_Matcher> ECC_PWE_Sample_Matcher<KDF, HMAC>::clone() const{
	return std::unique_ptr<Preimage_Matcher>(new ECC_PWE_Sample_Matcher<KDF, HMAC>(*this));
}


template <class KDF ,class HMAC>
bool ECC_PWE_Sample_Matcher<KDF, HMAC>::simulate_test(const std::string& password, const pwe_sample& sample){

	NTL::ZZ ecc_pwe = PWE_Sample_Matcher<KDF, HMAC>::derive_element(PWE_Sample_Matcher<KDF, HMAC>::kdf,
												PWE_Sample_Matcher<KDF, HMAC>::hmac, p, password, sample);
	bool result;
	if(ecc_pwe >= p){
		result = false;
	}
	else{
		// y_sqr_pwe = ecc_pwe^3 + a*ecc_pwe + b (mod p)
		NTL::ZZ y_sqr_pwe = NTL::AddMod(NTL::PowerMod(ecc_pwe, 3, p), NTL::AddMod(NTL::MulMod(a, ecc_pwe, p), b, p), p);
		// result = (is y_sqr_pwe a QR?)
		result = (NTL::Jacobi(y_sqr_pwe, p) == 1);
	}
	// return whether the result mathces the sample result
	return (result == (bool) sample.result);
}



#endif