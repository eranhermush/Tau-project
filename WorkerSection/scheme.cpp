#include "scheme.h"

string scheme::caesar_cipher(string plaintext, int offest){
	char c;
	for(int i = 0; i < plaintext.length(); i++){
		c = plaintext[i];
		if(c >= 'a' && c <= 'z'){
			c -= 'a';
			c = (c+offest) % 26;
			plaintext = 'a' + c;
		}
		else if(c >= 'A' && c <= 'Z'){
			c -= 'A';
			c = (c+offest) % 26;
			plaintext = 'A' + c;
		}
		else if(c >= '0' && c <= '9'){
			c -= '0';
			c = (c+offest) % 10;
			plaintext = '0' + c;
		}
	}
	return plaintext;
}