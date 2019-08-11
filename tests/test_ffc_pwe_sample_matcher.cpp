#include "../Worker/ffc_pwe_sample_matcher.h"
#include "../Worker/kdf_sae.h"
#include "../Worker/sae_groups.h"
#include "../Worker/pwe_sample.h"
#include "cryptopp/sha.h"
#include "cryptopp/hmac.h"
#include <NTL/ZZ.h>
#include <string>
#include <vector>
#include <iostream>

int main(){
	std::vector<std::string> spoofed_macs(8, "00:11:22:33:44:55");
	std::vector<std::string> target_macs(8, "ff:ee:dd:cc:bb:aa");
	std::vector<unsigned char> counters({4, 7, 10, 13, 16, 19, 22, 25});
	std::vector<char> results256({1, 0, 0, 0, 0, 1, 0, 0});
	std::vector<char> results384({0, 0, 1, 1, 1, 1, 1, 1});
	std::string correct("abcd");
	std::string incorrect("dcba");
	std::vector<pwe_sample> samples384 = pwe_sample::create_sample_vector(spoofed_macs, target_macs, counters, results384);

	CryptoPP::HMAC<CryptoPP::SHA256> hmac256;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>> kdf256(hmac256);
	CryptoPP::HMAC<CryptoPP::SHA384> hmac384;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA384>> kdf384(hmac384);

	for(unsigned int i = 0; i < 3; ++i){
		if(NTL::PowerMod(FFC::GROUPS[i].G, FFC::GROUPS[i].q, FFC::GROUPS[i].p) != 1){
			std::cout << "Group" << i+22 <<" parameters are incorrect" << std::endl;
		}
	}
	std::cout << "Groups 22, 23, 24 parameters are correct" << std::endl;


	FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt256_1(
		spoofed_macs, target_macs, counters, results256, kdf256, hmac256, FFC::GROUP_NAMES::GROUP22);

	FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt256_2(
		spoofed_macs, target_macs, counters, results256, kdf256, hmac256, FFC::GROUPS[0].p, FFC::GROUPS[0].G, FFC::GROUPS[0].q, false);

	FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA384>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt384_1(
		samples384, kdf384, hmac256, FFC::GROUP_NAMES::GROUP24);

	FFC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA384>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt384_2(
		samples384, kdf384, hmac256, FFC::GROUPS[2].p, FFC::GROUPS[2].G, FFC::GROUPS[2].q, false);

	if(!mt256_1.test(correct)){
		std::cout << "Group22: KDF with HMAC256 initialized by name: correct input failed" << std::endl;
	}
	if(mt256_1.test(incorrect)){
		std::cout << "Group22: KDF with HMAC256 initialized by name: incorrect input failed" << std::endl;
	}
	if(!mt256_2.test(correct)){
		std::cout << "Group22: KDF with HMAC256 initialized with values: correct input failed" << std::endl;
	}
	if(mt256_2.test(incorrect)){
		std::cout << "Group22: KDF with HMAC256 initialized with values: incorrect input failed" << std::endl;
	}
	std::cout << "finished testing with Group22" << std::endl;


	if(!mt384_1.test(correct)){
		std::cout << "Group24: KDF with HMAC384 initialized by name: correct input failed" << std::endl;
	}
	if(mt384_1.test(incorrect)){
		std::cout << "Group24: KDF with HMAC384 initialized by name: incorrect input failed" << std::endl;
	}
	if(!mt384_2.test(correct)){
		std::cout << "Group24: KDF with HMAC384 initialized with values: correct input failed" << std::endl;
	}
	if(mt384_2.test(incorrect)){
		std::cout << "Group24: KDF with HMAC384 initialized with values: incorrect input failed" << std::endl;
	}
	std::cout << "finished testing with Group24" << std::endl;



	std::unique_ptr<Preimage_Matcher> mt256_clone = mt256_1.clone();

	std::unique_ptr<Preimage_Matcher> mt384_clone = mt384_1.clone();

	if(!mt256_clone->test(correct)){
		std::cout << "Group22: clone: correct input failed" << std::endl;
	}
	if(mt256_clone->test(incorrect)){
		std::cout << "Group22: clone: incorrect input failed" << std::endl;
	}
	if(!mt384_clone->test(correct)){
		std::cout << "Group24: clone: correct input failed" << std::endl;
	}
	if(mt384_clone->test(incorrect)){
		std::cout << "Group24: clone: incorrect input failed" << std::endl;
	}

	std::cout << "finished testing with clones" << std::endl;

	return 0;
}