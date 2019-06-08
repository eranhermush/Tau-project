#ifndef TAU_PROJECT_NESTED_PASSWORD_GENERATOR_H
#define TAU_PROJECT_NESTED_PASSWORD_GENERATOR_H

#include <vector>
#include "password_generator.h"

#include <iostream>

class Nested_Password_Generator: public Password_Generator{
	public:
		Nested_Password_Generator(std::vector<std::unique_ptr<Password_Generator>>& generators, bool should_reset=true);

		/* Copy Constructor - copies the current state as well*/
		Nested_Password_Generator(const Nested_Password_Generator& generator);

		/* Returns a unique pointer to a new clone of the generator */
		std::unique_ptr<Password_Generator> clone() const;

		/* Advances the current password, if possible */
		void advance_password();

		/* Returns whether the generator has another password to produce */
		bool has_next() const{
			return has_next_flag;
		}

		/* Reverts the generator to its first password */
		void reset();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length() const{
			return false;
		}

	private:
		int gen_amount; /* how many generators are used */
		std::vector<std::unique_ptr<Password_Generator>> pass_generators;
		std::vector<int> pass_ranges; /* The first index of each subpassword */
		bool has_next_flag = false;

		void init();
		void update_password_suffix(int beginning_index);
};

#endif