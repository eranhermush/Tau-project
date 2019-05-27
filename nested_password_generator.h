#ifndef TAU_PROJECT_NESTED_PASSWORD_GENERATOR_H
#define TAU_PROJECT_NESTED_PASSWORD_GENERATOR_H

#include <vector>
#include "password_generator.h"
#include "char_password_generator.h"
#include "password_generator.h"

class Nested_Password_Generator: public Password_Generator{
	public:
		Nested_Password_Generator(std::vector<Password_Generator>& generators, bool should_reset=true);

		/* Advances the current password, if possible */
		void advance_password();

		/* Returns whether the generator has another password to produce */
		bool has_next();

		/* Reverts the generator to its original state right after its construction */
		void reset();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length(){
			return false;
		}

	private:
		std::vector<std::unique_ptr<Password_Generator>> pass_generators;
		std::vector<int> gen_order;
		std::vector<std::array<int, 2>> pass_ranges;
		bool has_next;

		void init();
}

#endif