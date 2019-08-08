#include "hash_matcher.h"
#include "cryptopp/sha.h"
#include <iostream>

class Dummy_Hash{
	public:
		Dummy_Hash(){}

		unsigned int DigestSize(){
			return 4;
		}

		void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t len){
			digest[0] = 'a';
			digest[1] = 'b';
			digest[2] = 'c';
			digest[3] = 'd';
			for(size_t i = 0; i < len && i < 4; ++i){
				digest[i] = input[i];
			}
		}
};

int main(){
	Hash_Matcher<Dummy_Hash> hm1(Dummy_Hash(), "abcd");
	Hash_Matcher<Dummy_Hash> hm2(Dummy_Hash(), "ebcd");
	Hash_Matcher<Dummy_Hash> hm3(Dummy_Hash(), "abab");
	
	std::unique_ptr<Preimage_Matcher> hmp = hm3.clone();

	if(!hm1.test("") || hm2.test("")){
		std::cout << "empty input failed" << std::endl;
	}
	if(!hm1.test("abcdef") || !hm2.test("e")){
		std::cout << "correct input failed" << std::endl;
	}
	if(hm1.test("bcdef") || hm2.test("aaaaaaa")){
		std::cout << "incorrect input failed" << std::endl;
	}
	if(!hm1.test("ed","edcd")){
		std::cout << "correct input with new target failed" << std::endl;
	}
	if(hm1.test("","") || hm1.test("","abcde")){
		std::cout << "wrong target size failed" << std::endl;
	}

	if(hmp->test("")){
		std::cout << "clone: empty input failed" << std::endl;
	}
	if(!hmp->test("abababab")){
		std::cout << "clone: correct input failed" << std::endl;
	}
	if(hmp->test("cdcd")){
		std::cout << "clone: incorrect input failed" << std::endl;
	}

	std::cout << "finished testing dummy hash" << std::endl;


	CryptoPP::SHA256 hash;
	std::string digest(32,0);
	std::string origin("this is the origin");
	hash.CalculateDigest((unsigned char*) &digest[0], (const unsigned char*) origin.data(), origin.size());
	Hash_Matcher<CryptoPP::SHA256> hm256(CryptoPP::SHA256(), digest);
	std::unique_ptr<Preimage_Matcher> hm256p = hm256.clone();

	if(hm256.test("")){
		std::cout << "empty input failed" << std::endl;
	}
	if(!hm256.test(origin)){
		std::cout << "correct input failed" << std::endl;
	}
	if(hm256.test(std::string("this is the origin", origin.size() + 1))){
		std::cout << "additional null byte  in input failed" << std::endl;
	}
	if(hm256.test("this isn't the origin")){
		std::cout << "incorrect input failed" << std::endl;
	}
	std::string digest2(32,0);
	hash.CalculateDigest((unsigned char*) &digest2[0], (const unsigned char*) "this isn't the origin", 21);
	if(!hm256.test("this isn't the origin", digest2) || hm256.test(origin, digest2)){
		std::cout << "new target failed" << std::endl;
	}
	if(hm256.test("hello","world") || hm256.test("","")){
		std::cout << "wrong target size failed" << std::endl;
	}

	if(hm256p->test("")){
		std::cout << "clone: empty input failed" << std::endl;
	}
	if(!hm256p->test(origin)){
		std::cout << "clone: correct input failed" << std::endl;
	}
	if(hm256p->test(std::string("this is the origin", origin.size() + 1))){
		std::cout << "clone: additional null byte  in input failed" << std::endl;
	}
	if(hm256p->test("this isn't the origin")){
		std::cout << "clone: incorrect input failed" << std::endl;
	}
	std::cout << "finished testing Crypto++ hash" << std::endl;
	return 0;
}