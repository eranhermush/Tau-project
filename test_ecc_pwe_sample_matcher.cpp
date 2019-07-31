#include "ecc_pwe_sample_matcher.h"
#include "kdf_sae.h"
#include "sae_groups.h"
#include "pwe_sample.h"
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
	std::vector<char> results256({0, 1, 1, 1, 1, 1, 0, 0});
	std::vector<char> results384({1, 1, 1, 0, 1, 1, 1, 1});
	std::string correct("abcd");
	std::string incorrect("dcba");
	std::vector<pwe_sample> samples384 = pwe_sample::create_sample_vector(spoofed_macs, target_macs, counters, results384);

	CryptoPP::HMAC<CryptoPP::SHA256> hmac256;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>> kdf256(hmac256);
	CryptoPP::HMAC<CryptoPP::SHA384> hmac384;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA384>> kdf384(hmac384);


	ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt256_1(
		spoofed_macs, target_macs, counters, results256, kdf256, hmac256, ECC::GROUP_NAMES::GROUP19);

	ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt256_2(
		spoofed_macs, target_macs, counters, results256, kdf256, hmac256, ECC::GROUPS[0].p, ECC::GROUPS[0].a, ECC::GROUPS[0].b);

	ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA384>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt384_1(
		samples384, kdf384, hmac256, ECC::GROUP_NAMES::GROUP21);

	ECC_PWE_Sample_Matcher<KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA384>>, CryptoPP::HMAC<CryptoPP::SHA256>> mt384_2(
		samples384, kdf384, hmac256, ECC::GROUPS[2].p, ECC::GROUPS[2].a, ECC::GROUPS[2].b);

	if(!mt256_1.test(correct)){
		std::cout << "Group19: KDF with HMAC256 initialized by name: correct input failed" << std::endl;
	}
	if(mt256_1.test(incorrect)){
		std::cout << "Group19: KDF with HMAC256 initialized by name: incorrect input failed" << std::endl;
	}
	if(!mt256_2.test(correct)){
		std::cout << "Group19: KDF with HMAC256 initialized with values: correct input failed" << std::endl;
	}
	if(mt256_2.test(incorrect)){
		std::cout << "Group19: KDF with HMAC256 initialized with values: incorrect input failed" << std::endl;
	}
	std::cout << "finished testing with Group19" << std::endl;


	if(!mt384_1.test(correct)){
		std::cout << "Group21: KDF with HMAC384 initialized by name: correct input failed" << std::endl;
	}
	if(mt384_1.test(incorrect)){
		std::cout << "Group21: KDF with HMAC384 initialized by name: incorrect input failed" << std::endl;
	}
	if(!mt384_2.test(correct)){
		std::cout << "Group21: KDF with HMAC384 initialized with values: correct input failed" << std::endl;
	}
	if(mt384_2.test(incorrect)){
		std::cout << "Group21: KDF with HMAC384 initialized with values: incorrect input failed" << std::endl;
	}
	std::cout << "finished testing with Group21" << std::endl;



	std::unique_ptr<Preimage_Matcher> mt256_clone = mt256_1.clone();

	std::unique_ptr<Preimage_Matcher> mt384_clone = mt384_1.clone();

	if(!mt256_clone->test(correct)){
		std::cout << "Group19: clone: correct input failed" << std::endl;
	}
	if(mt256_clone->test(incorrect)){
		std::cout << "Group19: clone: incorrect input failed" << std::endl;
	}
	if(!mt384_clone->test(correct)){
		std::cout << "Group21: clone: correct input failed" << std::endl;
	}
	if(mt384_clone->test(incorrect)){
		std::cout << "Group21: clone: incorrect input failed" << std::endl;
	}

	std::cout << "finished testing with clones" << std::endl;

	return 0;
}