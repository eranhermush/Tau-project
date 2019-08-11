#include "../Worker/preimage_seeker.h"
#include "../Worker/file_password_generator.h"
#include "../Worker/ecc_pwe_sample_matcher.h"
#include "../Worker/kdf_sae.h"
#include "../Worker/sae_groups.h"
#include "../Worker/pwe_sample.h"
#include "cryptopp/sha.h"
#include "cryptopp/hmac.h"
#include <string>
#include <vector>

int main(){
	std::vector<std::string> spoofed_macs({"65:61:02:56:01:96", "65:61:02:56:01:96", "65:61:02:56:01:96", "65:61:02:56:01:96",
		"65:61:02:56:01:63","65:61:02:56:01:af", "65:61:02:56:01:af", "65:61:02:56:01:af", "65:61:02:56:01:af", "65:61:02:56:01:2f",
		"65:61:02:56:01:2f", "65:61:02:56:01:76", "65:61:02:56:01:8b", "65:61:02:56:01:8b", "65:61:02:56:01:8b", "65:61:02:56:01:8b",
		"65:61:02:56:01:8b", "65:61:02:56:01:8b", "65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:10",
		"65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:10", 
		"65:61:02:56:01:10", "65:61:02:56:01:10", "65:61:02:56:01:45", "65:61:02:56:01:45", "65:61:02:56:01:45", "65:61:02:56:01:45",
		"65:61:02:56:01:23", "65:61:02:56:01:7f", "65:61:02:56:01:79", "65:61:02:56:01:f9"});
	std::vector<std::string> target_macs(38, "01:04:09:16:25:36");
	std::vector<unsigned char> counters({1, 2, 3, 4, 1, 1, 2, 3, 4, 1, 2, 1, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		11, 12, 1, 2, 3, 4, 1, 1, 1, 1});
	std::vector<char> results({0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
		0, 1, 1, 1, 1, 0});
	std::string correct("ghosts");

	CryptoPP::HMAC<CryptoPP::SHA256> hmac256;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>> kdf256(hmac256);


	ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt256(
		spoofed_macs, target_macs, counters, results, kdf256, hmac256, ECC::GROUP_NAMES::GROUP19);

	// 150,000 passwords
	File_Password_Generator gen("./rockyou.txt", 0, 1231380);

	Preimage_Seeker ps(gen, mt256, false);

	std::vector<std::string> matches(ps.seek_all());

	
	if(matches.size() != 1 || matches[0] != "ghosts"){
		return 1;
	}

	return 0;
}