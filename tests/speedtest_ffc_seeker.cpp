#include "../Worker/preimage_seeker.h"
#include "../Worker/char_password_generator.h"
#include "../Worker/ffc_pwe_sample_matcher.h"
#include "../Worker/kdf_sae.h"
#include "../Worker/sae_groups.h"
#include "../Worker/pwe_sample.h"
#include "cryptopp/sha.h"
#include "cryptopp/hmac.h"
#include <string>
#include <vector>

int main(){
	std::vector<std::string> spoofed_macs({"65:61:02:56:01:d5", "65:61:02:56:01:6f", "65:61:02:56:01:5a", "65:61:02:56:01:79",
		"65:61:02:56:01:79", "65:61:02:56:01:d7", "65:61:02:56:01:47", "65:61:02:56:01:47", "65:61:02:56:01:ac", "65:61:02:56:01:f6",
		"65:61:02:56:01:e9", "65:61:02:56:01:e9", "65:61:02:56:01:a7", "65:61:02:56:01:a7", "65:61:02:56:01:a7", "65:61:02:56:01:38",
		"65:61:02:56:01:02", "65:61:02:56:01:95", "65:61:02:56:01:fa", "65:61:02:56:01:fa", "65:61:02:56:01:71", "65:61:02:56:01:71",
		"65:61:02:56:01:e2", "65:61:02:56:01:f9", "65:61:02:56:01:9e", "65:61:02:56:01:90", "65:61:02:56:01:90", "65:61:02:56:01:79",
		"65:61:02:56:01:79", "65:61:02:56:01:d5", "65:61:02:56:01:8d", "65:61:02:56:01:72", "65:61:02:56:01:72", "65:61:02:56:01:36",
		"65:61:02:56:01:36", "65:61:02:56:01:36", "65:61:02:56:01:36", "65:61:02:56:01:83"});
	std::vector<std::string> target_macs(38, "01:04:09:16:25:36");
	std::vector<unsigned char> counters({1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1,
		2, 1, 1, 1, 2, 1, 2, 3, 4, 1});
	std::vector<char> results({1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1,
		0, 1, 0, 0, 0, 1, 1});
	std::string correct("Veep");

	CryptoPP::HMAC<CryptoPP::SHA256> hmac256;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>> kdf256(hmac256);


	FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt256(
		spoofed_macs, target_macs, counters, results, kdf256, hmac256, FFC::GROUP_NAMES::GROUP22);

	Char_Pattern_Password_Generator gen("Cccc", 0, 456975);

	Preimage_Seeker ps(gen, mt256, false);

	std::vector<std::string> matches(ps.seek_all());

	if(matches.size() != 1 || matches[0] != "Veep"){
		return 1;
	}

	return 0;
}