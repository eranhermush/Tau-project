#ifndef TAU_PROJECT_FFC_PWE_SAMPLE_MATCHER_H
#define TAU_PROJECT_FFC_PWE_SAMPLE_MATCHER_H

#include "sae_groups.h"
#include "pwe_sample_matcher.h"
#include <NTL/ZZ.h>

/*
	FFC_PWE_Sample_Matcher is PWE_Sample_Matcher with password-to-element function
	which maps passwords to elements of a prime finite field.

	The same requirements of the KDF and HMAC parameters hold.
*/

/* 
	For a sample of FFC_PWE, (bool) sample.result <==> (the derived element is valid)
	Which means 1) it is smaller than the prime modulus p
				2) it is a q-th power (q is the prime order of the subgroup)

	Notice that if g is a generator of the whole group (Z/pZ)* ~ (Z/(p-1)Z, +), then for
	a uniformly choosen element x = g^e in (Z/pZ)* (e ~ Uniform{0,...,p-1}) it holds that
	Pr[x fails (2)] = Pr[(g^e) ^ ((p-1)/q) = 1 (mod p)] =
					= Pr[e*(p-1)/q = k*(p-1) for some natural k] = Pr[q|e] =
					= (p-1)/q * 1/(p-1) = 1/q
	Hence when the 1 <<< q the probabilty that x fails (2) conditioned on passing (1) is
	negligible.
	With Group22\Group23\Group24 q is atleast 160 bits.
*/

template <class KDF ,class HMAC>
class FFC_PWE_Sample_Matcher: public PWE_Sample_Matcher<KDF, HMAC>{
	public:
		~FFC_PWE_Sample_Matcher() =default;

		/* Constructor
			Uses a sample vector and group paramters */
		FFC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj,
			const HMAC& hmac_obj, const NTL::ZZ& prime, const NTL::ZZ& subgroup_gen=1,
			const NTL::ZZ& subgroup_order=1, bool prob_negligible=true);

		/* Constructor
			Uses a sample vector and group name */
		FFC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj,
			const HMAC& hmac_obj, FFC::GROUP_NAMES name, bool prob_negligible=true);

		/* Constructor
			Uses a samples parameters and group paramters */
		FFC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs, const std::vector<std::string>& target_macs,
			const std::vector<unsigned char>& counters, const std::vector<char>& results, const KDF& kdf_obj,
			const HMAC& hmac_obj, const NTL::ZZ& prime, const NTL::ZZ& subgroup_gen=1, const NTL::ZZ& subgroup_order=1,
			bool prob_negligible=true);

		/* Constructor
			Uses a samples parameters and group name */
		FFC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs, const std::vector<std::string>& target_macs,
			const std::vector<unsigned char>& counters, const std::vector<char>& results, const KDF& kdf_obj,
			const HMAC& hmac_obj, FFC::GROUP_NAMES name, bool prob_negligible=true);


		/* Copy constructor */
		FFC_PWE_Sample_Matcher(const FFC_PWE_Sample_Matcher& ffc_matcher);

		std::unique_ptr<Preimage_Matcher> clone() const;

		/* tests a password with a single sample */
		bool simulate_test(const std::string& password, const pwe_sample& sample);


	protected:
		NTL::ZZ p;
		NTL::ZZ g;
		NTL::ZZ q;
		bool negligible = true;

	private:
		// sorts samples by result for faster eliminations
		void sort_samples();
};





/*		Implementation		*/

/* Constructors */

template <class KDF ,class HMAC>
FFC_PWE_Sample_Matcher<KDF, HMAC>::FFC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj,
	const HMAC& hmac_obj, const NTL::ZZ& prime, const NTL::ZZ& subgroup_gen, const NTL::ZZ& subgroup_order, bool prob_negligible)
	: PWE_Sample_Matcher<KDF ,HMAC>(samps, kdf_obj, hmac_obj), p(prime),
	g(subgroup_gen), q(subgroup_order), negligible(prob_negligible){

		sort_samples();
	}


template <class KDF ,class HMAC>
FFC_PWE_Sample_Matcher<KDF, HMAC>::FFC_PWE_Sample_Matcher(const std::vector<pwe_sample>& samps, const KDF& kdf_obj,
	const HMAC& hmac_obj, FFC::GROUP_NAMES name, bool prob_negligible)
	: FFC_PWE_Sample_Matcher<KDF ,HMAC>(samps, kdf_obj, hmac_obj, FFC::GROUPS[(int)name].p,
		FFC::GROUPS[(int)name].G, FFC::GROUPS[(int)name].q, prob_negligible) {}


template <class KDF ,class HMAC>
FFC_PWE_Sample_Matcher<KDF, HMAC>::FFC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs,
	const std::vector<std::string>& target_macs, const std::vector<unsigned char>& counters, const std::vector<char>& results,
	const KDF& kdf_obj,	const HMAC& hmac_obj, const NTL::ZZ& prime, const NTL::ZZ& subgroup_gen,
	const NTL::ZZ& subgroup_order, bool prob_negligible)
	: PWE_Sample_Matcher<KDF ,HMAC>(spoofed_macs, target_macs, counters, results, kdf_obj, hmac_obj), p(prime),
	g(subgroup_gen), q(subgroup_order), negligible(prob_negligible){

		sort_samples();
	}


template <class KDF ,class HMAC>
FFC_PWE_Sample_Matcher<KDF, HMAC>::FFC_PWE_Sample_Matcher(const std::vector<std::string>& spoofed_macs,
	const std::vector<std::string>& target_macs, const std::vector<unsigned char>& counters, const std::vector<char>& results,
	const KDF& kdf_obj,	const HMAC& hmac_obj, FFC::GROUP_NAMES name, bool prob_negligible)
	: FFC_PWE_Sample_Matcher<KDF ,HMAC>(spoofed_macs, target_macs, counters, results, kdf_obj, hmac_obj,
		FFC::GROUPS[(int)name].p, FFC::GROUPS[(int)name].G, FFC::GROUPS[(int)name].q, prob_negligible) {}


template <class KDF ,class HMAC>
FFC_PWE_Sample_Matcher<KDF, HMAC>::FFC_PWE_Sample_Matcher(const FFC_PWE_Sample_Matcher& ffc_matcher)
	: PWE_Sample_Matcher<KDF ,HMAC>(ffc_matcher.samples, ffc_matcher.kdf, ffc_matcher.hmac), p(ffc_matcher.p),
	g(ffc_matcher.g), q(ffc_matcher.q), negligible(ffc_matcher.negligible) {}



/* */

template <class KDF ,class HMAC>
std::unique_ptr<Preimage_Matcher> FFC_PWE_Sample_Matcher<KDF, HMAC>::clone() const{
	return std::unique_ptr<Preimage_Matcher>(new FFC_PWE_Sample_Matcher<KDF, HMAC>(*this));
}



template <class KDF ,class HMAC>
void FFC_PWE_Sample_Matcher<KDF, HMAC>::sort_samples(){
	// Pr[FFC_PWE > P] < 0.5 therefore, placing samples with a negative result first would cause faster diqualifications on average
	std::stable_sort(FFC_PWE_Sample_Matcher<KDF, HMAC>::samples.begin(), FFC_PWE_Sample_Matcher<KDF, HMAC>::samples.end(),
			[](const pwe_sample& lhs, const pwe_sample& rhs){ return lhs.result <= rhs.result;});
}


/* FFC test */

template <class KDF ,class HMAC>
bool FFC_PWE_Sample_Matcher<KDF, HMAC>::simulate_test(const std::string& password, const pwe_sample& sample){

	NTL::ZZ ffc_pwe = PWE_Sample_Matcher<KDF, HMAC>::derive_element(PWE_Sample_Matcher<KDF, HMAC>::kdf,
												PWE_Sample_Matcher<KDF, HMAC>::hmac, p, password, sample);
	bool result;
	if(ffc_pwe >= p){
		result = false;
	}
	else{
		if(negligible){
			// if the subgroup order is large enough then the probability that element is invalid
			// is negligible (once  it is smaller than p)
			result = true;
		}
		else{
			// check that the element is a q-th power
			NTL::ZZ ffc_final_pwe = NTL::PowerMod(ffc_pwe, (p-1)/q, p);
			result = (ffc_final_pwe != 1);
		}
	}
	// return whether the result mathces the sample result
	return (result == (bool) sample.result);
}



#endif