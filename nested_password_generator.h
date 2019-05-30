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
		bool has_next(){
			return has_next_flag;
		}

		/* Reverts the generator to its first password */
		void reset();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length(){
			return false;
		}

	private:
		int gen_amount; /* how many generators are used */
		std::vector<std::unique_ptr<Password_Generator>> pass_generators;
		std::vector<std::array<int, 2>> pass_ranges;
		bool has_next_flag = false;

		void init();
		void update_paswword_suffix(int beginning_index);
}

#endif