
#include <iostream>
#include <string>
using namespace std;

class scheme{

	/* This function cpmputes the caesar cipher with a given offest.
		 @param plaintext - the text to be encrypted
		 @param offest - the offest of the code
	*/
public:
	static string caesar_cipher(string plaintext, int offest);
};