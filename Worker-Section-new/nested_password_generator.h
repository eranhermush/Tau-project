#ifndef TAU_PROJECT_NESTED_PASSWORD_GENERATOR_H
#define TAU_PROJECT_NESTED_PASSWORD_GENERATOR_H

#include <vector>
#include "password_generator.h"
#include <cstdint>
#include <algorithm>

/* A password generator that concatenates a few simpler generators
	Has a higher complexity cost than simpler generator,
	Therefore avoid using if possible, and avoid a doubly nested generator */

class Nested_Password_Generator: public Password_Generator{
	public:
		/* Constuctors a nested generator with in the given range */
		Nested_Password_Generator(const std::vector<std::unique_ptr<Password_Generator>>& generators,
			const std::vector<uint64_t>& start_pos, const std::vector<uint64_t>& end_pos);

		/* Constuctors a nested generator with in the given range, where total_start and total_end
			are converted to vectors */
		Nested_Password_Generator(const std::vector<std::unique_ptr<Password_Generator>>& generators,
			uint64_t total_start, uint64_t total_end);

		/* Copy Constructor - copies the current state as well*/
		Nested_Password_Generator(const Nested_Password_Generator& generator);

		/* Returns a unique pointer to a new clone of the generator */
		std::unique_ptr<Password_Generator> clone() const;

		/* Advances the current password, if possible */
		void advance_password();

		/* Sets the current password to be the one at position (position should be in the range) */
		void set_password(const std::vector<uint64_t>& new_pos);

		void set_password(uint64_t position){
			std::vector<uint64_t> new_pos(gen_amount, 0);
			number_to_vector(position, generator_lengths, new_pos);
			set_password(new_pos);
		}

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

		/* Returns the length of the current range
			Avoid using unless you are sure why, the units are instance dependent */
		uint64_t get_range_length() const{
			return total_end_pos - total_start_pos + 1;
		}

		void expand_to_max_range();

		/* Returns the current position in the range (from 0 *not* from reset)
			Avoid using unless you are sure why, high complexity cost and not much use*/
		uint64_t get_curr_position() const{
			return vector_to_number(generator_lengths, curr_pos_vector);
		}

	private:
		int gen_amount; /* how many generators are used */
		std::vector<std::unique_ptr<Password_Generator>> pass_generators;
		std::vector<int> pass_ranges; /* The first index of each subpassword */
		std::vector<uint64_t> generator_lengths;
		std::vector<uint64_t> curr_pos_vector;
		std::vector<uint64_t> end_pos_vector;
		std::vector<uint64_t> start_pos_vector;
		uint64_t total_start_pos;
		uint64_t total_end_pos;
		bool has_next_flag = false;

		static uint64_t vector_to_number(const std::vector<uint64_t>& digits, const std::vector<uint64_t>& bases);
		static void number_to_vector(uint64_t number, const std::vector<uint64_t>& bases, std::vector<uint64_t>& dest); // puts number representation in dest
																												// dest and basis must be of the same size
		
		void init_generators(const std::vector<std::unique_ptr<Password_Generator>>& generators); // assumes gen_amount is already correct
		void init_vectors(const std::vector<uint64_t>& start_pos, const std::vector<uint64_t>& end_pos); // assumes gen_amount, pass_generators,
																										// generator_lengths are already correct
		void init_password();
		void update_password_suffix(int beginning_index);
};

#endif