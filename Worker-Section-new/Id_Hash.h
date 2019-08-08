#ifndef ID_HASH_H_TAU
#define ID_HASH_H_TAU

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>



class Id_Hash
{
	public:
		Id_Hash();

		unsigned int DigestSize();

		void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t len);
};
#endif