#ifndef TAU_PROJECT_CHAR_PASSWORD_GENERATOR_H
#define TAU_PROJECT_CHAR_PASSWORD_GENERATOR_H

#include <vector>
#include <functional>
#include <iostream>
#include <cstdint>
#include "password_generator.h"

static std::vector<char> DATA_LOWERCASE({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});
static std::vector<char> DATA_UPPERCASE({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});
static std::vector<char> DATA_DIGITS({'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'});
static std::vector<char> DATA_PUNCTUATION({'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'});

/*---------------------------------------------------------------------------*/

/* Char_Pattern_Password_Generator excracts passwords matching a fixed length pattern in a certain range */

/* The supported patterns consist of the characters 'c', 'C', 'd', 'p', 'x' which indicate:
	'c' = a lowercase charcater
	'C' = an uppercase charcater
	'd' = a digit
	'p' = a punctuation mark
	'x' = a character from some specified data

	For example: "aaAA0!"" matches the pattern "ccCCdp"
*/

/* Strings matching a pattern are ordered by lexicographic order (where leftmost is most significant) and are zero based.
	
	For example: the 0-th string matching "ddd" is "000" and the 128-th is "128"
*/

class Char_Pattern_Password_Generator: public Password_Generator{
	public:
		/* rep_string is the pattern string consisting only of 'c'\'C'\'d'\'p' 
			first is the index of the first password to be loaded
			last is the index of the last unique password to be produced */
		Char_Pattern_Password_Generator(const std::string& rep_string, uint64_t first, uint64_t last);

		/* rep_string is the pattern string consisting of 'c'\'C'\'d'\'p'\'x'
			data is a vector containing characters to be matched for the 'x' symbols
			rep_indices is a vector with size = the number of appearances of 'x' in rep_string, where each
				of its entries is the index of the data to be matched for this appearance of 'x' 
			first is the index of the first password to be loaded
			last is the index of the last unique password to be produced

			For example: calling with ("xddxx",{0,1,0},{{'#'}, {'0','2','4','6','8'}}, 0, 499) would
						construct a generator for even 3-digit numbers between two '#' */
		Char_Pattern_Password_Generator(const std::string& rep_string, const std::vector<int>& rep_indices,
											 const std::vector<std::vector<char>>& data, uint64_t first, uint64_t last);

		/* Copy constructor - also copies the current password and state */
		Char_Pattern_Password_Generator(const Char_Pattern_Password_Generator& generator);

		/* Returns a unique pointer to a new clone of the generator */
		std::unique_ptr<Password_Generator> clone() const;

		/* Advances the current password to the next one (if one exists in the range) */
		void advance_password();

		/* Sets the current password to be the one at position (position should be in the range)*/
		void set_password(uint64_t position);

		/* Returns whether the generator has more passwords in its range */
		bool has_next() const;

		/* Reverts to the first password */
		void reset();

		/* Expands the password range to the widest possible of the generator
			Does not change the current password */
		void expand_to_max_range();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length() const{
			return true;
		}

		uint64_t get_first_position() const{
			return first;
		}

		uint64_t get_last_position() const{
			return last;
		}

		uint64_t get_range_length() const{
			return last - first + 1;
		}

		uint64_t get_curr_position() const{
			return curr_position;
		}

	private:
		int length;
		uint64_t first;
		uint64_t last;
		uint64_t curr_position;
		std::vector<int> indices;
		std::vector<std::reference_wrapper<std::vector<char>>> char_sources;
		std::vector<int> sources_index; /* stores the index of the source for each characther, used for copying (negative values imply the static data) */
		std::vector<std::vector<char>> additional_data; /* use is optional */

		void init(const std::string& rep_string, const std::vector<int>& rep_indices);
};


/*---------------------------------------------------------------------------*/

#endif