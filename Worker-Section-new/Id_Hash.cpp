#include "Id_Hash.h"
Id_Hash::Id_Hash()
{}


unsigned int Id_Hash::DigestSize(){
	return 5;
}

void Id_Hash::CalculateDigest(unsigned char* digest, const unsigned char* input, size_t len){
	for(size_t i = 0; i < 5; ++i){
		digest[i] = '0';
	}
	for(size_t i = 0; i < len && i < 5; ++i){
		digest[i] = input[i];
	}
}