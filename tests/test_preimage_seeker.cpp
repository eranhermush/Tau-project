#include "../Worker/preimage_seeker.h"
#include "../Worker/char_password_generator.h"
#include "../Worker/hash_matcher.h"
#include <string>
#include <vector>
#include <iostream>
#include "cryptopp/whrlpool.h"

class Dummy_Hash{
	public:
		Dummy_Hash(){}

		unsigned int DigestSize(){
			return 2;
		}

		void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t len){
			digest[0] = '0';
			digest[1] = '0';
			for(size_t i = 0; i < len && i < 2; ++i){
				digest[i] = input[i];
			}
		}
};


int main(){
	std::string res_str;
	std::vector<std::string> res_vec;
	bool found;

	Hash_Matcher<Dummy_Hash> hm(Dummy_Hash(), "99");
	Char_Pattern_Password_Generator cpg1("ccc", 0, 26*26*26-1);
	Char_Pattern_Password_Generator cpg2("ddd", 0, 999);
	Preimage_Seeker psc(cpg1, hm, false);
	Preimage_Seeker psd(cpg2, hm, false);

	res_str = psd.seek_next(found);
	if(!found || std::string("990").compare(0, 3, res_str)){
		std::cout << "seek_next: failed existing target" << std::endl;
	}
	psd.reset();

	res_str = psc.seek_next(found);
	if(found || res_str != ""){
		std::cout << "seek_next: failed non-existing target" << std::endl;
	}
	psc.reset();

	res_vec = psd.seek_all();
	if(res_vec.size() != 10 || std::string("991").compare(0, 3, res_vec[1])){
		std::cout << "seek_all: failed existing target" << std::endl;
	}
	psd.reset();

	res_vec = psc.seek_all();
	if(res_vec.size() > 0){
		std::cout << "seek_all: failed non-existing target" << std::endl;
	}
	psc.reset();

	std::cout << "finished testing with dummy hash and different generators" << std::endl;




	CryptoPP::Whirlpool hash;
	std::string target1(hash.DigestSize(), 0);
	hash.CalculateDigest((unsigned char*) &target1[0], (const unsigned char*) "abc", 3);

	std::string target2(hash.DigestSize(), 0);
	hash.CalculateDigest((unsigned char*) &target2[0], (const unsigned char*) "Abc", 3);
	
	Hash_Matcher<CryptoPP::Whirlpool> hm1(CryptoPP::Whirlpool(), target1);
	Hash_Matcher<CryptoPP::Whirlpool> hm2(CryptoPP::Whirlpool(), target2);
	Hash_Matcher<CryptoPP::Whirlpool> hm3(CryptoPP::Whirlpool(), target1.substr(0,32));

	Char_Pattern_Password_Generator cpg("ccc", 0, 26*26*26-1);
	Preimage_Seeker ps1(cpg, hm1, false);
	Preimage_Seeker ps2(cpg, hm2, false);
	Preimage_Seeker ps3(cpg, hm3, false);


	res_str = ps1.seek_next(found);
	if(!found || std::string("abc").compare(0, 3, res_str)){
		std::cout << "seek_next: failed existing target" << std::endl;
	}
	ps1.reset();

	res_str = ps2.seek_next(found);
	if(found || res_str != ""){
		std::cout << "seek_next: failed non-existing target" << std::endl;
	}
	ps2.reset();

	res_str = ps3.seek_next(found);
	if(found || res_str != ""){
		std::cout << "seek_next: failed wrong size target" << std::endl;
	}
	ps3.reset();

	res_vec = ps1.seek_all();
	if(res_vec.size() < 1 || std::string("abc").compare(0, 3, res_vec[0])){
		std::cout << "seek_all: failed existing target" << std::endl;
	}
	ps1.reset();

	res_vec = ps2.seek_all();
	if(res_vec.size() > 0){
		std::cout << "seek_all: failed non-existing target" << std::endl;
	}
	ps2.reset();

	res_vec = ps3.seek_all();
	if(res_vec.size() > 0){
		std::cout << "seek_all: failed wrong size target" << std::endl;
	}
	ps3.reset();

	std::cout << "finished testing with Crypto++ hash and different targets" << std::endl;


	return 0;
}