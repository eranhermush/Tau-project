#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

class hash_functions
{
	public:


	    static void id_function(std::string& password);

};

class Id_Hash{
	public:
		Id_Hash(){}

		unsigned int DigestSize(){
			return 16;
		}

		void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t len){
			for(size_t i = 0; i < 16; ++i){
				digest[i] = '0';
			}
			for(size_t i = 0; i < len && i < 16; ++i){
				digest[i] = input[i];
			}
		}
};





#endif