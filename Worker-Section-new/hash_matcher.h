#ifndef TAU_PROJECT_HASH_MATCHER_H
#define TAU_PROJECT_HASH_MATCHER_H

#include "preimage_matcher.h"

/* Hash_Matcher is a Preimage_Matcher for hash functions without parameters

	The template parameter H represents a hash function object, namely it should implement
	 1) The function: unsigned int DigestSize()
	 	(expected to return the hash digest size in bytes)
	 2) The function: void CalculateDigest(unsigned char* digest, const unsigned char* input, size_t input_length)
		(expected to calculate the hash of input and write the result in digest)
	
	For example H can be a Crypto++ class which implements the HashTransformation interface
*/

template<class H>
class Hash_Matcher: public Preimage_Matcher{
	public:
		/* Constructor
			target_image is the default value to match with*/
		Hash_Matcher(const H& hash_object, const std::string& target_image);

		/* Copy constructor */
		Hash_Matcher(const Hash_Matcher<H>& matcher);

		virtual std::unique_ptr<Preimage_Matcher> clone() const;

		/* Tests whether the input is a valid preimage of image*/
		virtual bool test(const std::string& input, const std::string& image);

		/* Tests whether the input is a valid preimage of the saved target*/
		virtual bool test(const std::string& input);

	protected:
		H hash;
		std::string target;
		std::string digest;
};


/*		implementation		*/

template<class H>
Hash_Matcher<H>::Hash_Matcher(const H& hash_object, const std::string& target_image)
	: hash(hash_object), target(target_image){
	digest.resize(hash.DigestSize());
}

template<class H>
Hash_Matcher<H>::Hash_Matcher(const Hash_Matcher<H>& matcher) : Hash_Matcher<H>(matcher.hash, matcher.target){}

template<class H>
std::unique_ptr<Preimage_Matcher> Hash_Matcher<H>::clone() const{
	return std::unique_ptr<Preimage_Matcher>(new Hash_Matcher<H>(*this));
}

template<class H>
bool Hash_Matcher<H>::test(const std::string& input, const std::string& image){
	// make sure sizes are correct
	unsigned int digest_size = hash.DigestSize();
	if(image.size() != digest_size){
		return false;
	}
	if(digest.size() != digest_size){
		digest.resize(digest_size);
	}
	// compute the hash
	hash.CalculateDigest((unsigned char*) &digest[0], (const unsigned char*) input.data(), input.size());
	// test if the input matches the image
	return (image.compare(0, digest_size, digest) == 0);
}

template<class H>
bool Hash_Matcher<H>::test(const std::string& input){
	return test(input, target);
}

#endif