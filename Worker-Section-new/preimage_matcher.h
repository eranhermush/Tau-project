#ifndef TAU_PROJECT_PREIMAGE_MATCHER_H
#define TAU_PROJECT_PREIMAGE_MATCHER_H

#include <string>
#include <memory>
	
/* Preimage_Matcher is an interface for checking whether inputs yeild a specific output by some function*/
class Preimage_Matcher{
	public:
		virtual ~Preimage_Matcher() =default;

		/* Copies the Preimage_Matcher object and returns a unique pointer to the copy*/
		virtual std::unique_ptr<Preimage_Matcher> clone() const =0;

		/* Tests whether the input is a valid preimage*/
		virtual bool test(const std::string& input) =0;
};

#endif