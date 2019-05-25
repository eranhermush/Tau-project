#ifndef TAU_PROJECT_PASSWORD_GENERATOR_H
#define TAU_PROJECT_PASSWORD_GENERATOR_H

#include <string>

/* Password_Generator is a basic interface for a password generator object */
class Password_Generator{
	public:

		/* Returns a reference to the curent password */
		virtual const std::string& get_password(){
			return curr_password;
		}

		/* Advances the current password, if possible */
		virtual void advance_password() =0;

		/* Returns whether the generator has another password to produce */
		virtual bool has_next() =0;

		/* Reverts the generator to its original state right after its construction */
		virtual void reset() =0;

		/* Returns whether the generator produces passwords of fixed length */
		virtual bool has_fixed_length() =0;

	protected:
		std::string curr_password;
};


#endif