#include "kdf_sae.h"
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include <string>
#include <iostream>

class Dummy_HMAC{
	public:
		unsigned int DigestSize(){
			return 8;
		}

		void SetKey(const unsigned char* key, size_t key_length){
			unsigned char k = key[key_length - 1];
			k++;
			return;
		}

		void CalculateDigest(unsigned char* digest, const unsigned char* input, unsigned int input_length){
			//std::cout << "input length: " << input_length << std::endl;
			for(unsigned int i = 0; i < 8 && i < input_length; ++i){
				//std::cout << "copying " << (int) input[i] << " to " << i << " position" << std::endl;
				digest[i] = input[i];
			}
		}
};

int main(){
	Dummy_HMAC hmac;
	KDF_SAE<Dummy_HMAC> kdf(hmac);
	
	std::string answer_128("\x01\x00llcc\x80\x00\x02\x00llcc\x80\x00", 16);
	std::string result_128(kdf.derive_key("does not matter", "ll", "cc", 128));
	if(answer_128.compare(0, 16, result_128) != 0){
		std::cout << "Dummy HMAC: failed 128 bits key" << std::endl;
	}

	std::string answer_198("\x00@\x1b\x1b\x18\xd8\xf1\x80\x00\x80\x1b\x1b\x18\xd8\xf1\x80\x00\xc0\x1b\x1b\x18\xd8\xf1\x80\x01", 25);
	std::string result_198(kdf.derive_key("does not matter", "ll", "cc", 198));
	if(answer_198.compare(0, 25, result_198) != 0){
		std::cout << "Dummy HMAC: failed 198 bits key" << std::endl;
	}

	std::string answer_197("\x00 \r\x8d\x8clx\xa0\x00@\r\x8d\x8clx\xa0\x00`\r\x8d\x8clx\xa0\x00", 25);
	std::string result_197(kdf.derive_key("does not matter", "ll", "cc", 197));
	if(answer_197.compare(0, 25, result_197) != 0){
		std::cout << "Dummy HMAC: failed 197 bits key" << std::endl;
	}

	std::cout << "finished testing with Dummy HMAC" << std::endl;


	CryptoPP::HMAC<CryptoPP::SHA256> hmac256;
	KDF_SAE<CryptoPP::HMAC<CryptoPP::SHA256>> kdf256(hmac256);

	std::string answer_512("\x81\x76\x1f\x88\x2e\x4d\x13\x68\x80\xe3\xde\x37\xb6\xc6\xd1\x4f\x60\x76\x34\xa6\xe0\xfa\xb2\x29\x20\xc8\xa3\x87\xa5\x71\xe0\xe1\x34\x46\xc6\x65\x93\x81\x83\x50\x45\xe8\x1d\x2c\x9d\xc8\x1c\x52\xf7\xb2\x7b\x22\x44x\xce\xb3p/\xfd\xd3\x99\xef<\xd8", 64);
	std::cout << "answer_512 size should be 64: " << answer_512.size() << std::endl;
	std::string result_512(kdf256.derive_key("red", "green", "blue", 512));
	if(answer_512.compare(0, 64, result_512) != 0){
		std::cout << "Crypto++ HMAC-SHA256: failed 512 bits key" << std::endl;
	}

	std::string answer_516("\r\x13q\x3c\x96\t\x1f\xc4Sq&\"Y\xb4\xc7\xd8\x93\xda\x88M\x1a\xd7\xc5W\x08\xad\x94\xe1#L\xc5\x1d\xd8\xd5\xdc\xe6\xaa\xbfN\x12\x7f\xb2\x87\x2dLQ\xa6\xa0{\xaf|\x3e\xe7\xf5\xee\x98yUB\x13O\xeb\xab\xac\xf3", 65);
	std::cout << "answer_516 size should be 65: " << answer_516.size() << std::endl;
	std::string result_516(kdf256.derive_key("red", "green", "blue", 516));
	if(answer_516.compare(0, 65, result_516) != 0){
		std::cout << "Crypto++ HMAC-SHA256: failed 516 bits key" << std::endl;
	}

	std::cout << "finished testing with Crypto++ HMAC-SHA256" << std::endl;
}