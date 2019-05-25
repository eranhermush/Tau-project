#ifndef TAU_PROJECT_CHAR_PASSWORD_GENERATOR_H
#define TAU_PROJECT_CHAR_PASSWORD_GENERATOR_H

#include <vector>
#include <functional>
#include <iostream>
#include "password_generator.h"

static std::vector<char> DATA_LOWERCASE({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});
static std::vector<char> DATA_UPPERCASE({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});
static std::vector<char> DATA_DIGITS({'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'});
static std::vector<char> DATA_PUNCTUATION({'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'});

/*---------------------------------------------------------------------------*/

/* Fixed_Length_Char_Password_Generator excracts passwords matching a fixed length pattern in a certain range */

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

class Fixed_Length_Char_Password_Generator: public Password_Generator{
	public:
		/* rep_string is the pattern string consisting only of 'c'\'C'\'d'\'p' 
			first is the index of the first password to be loaded
			last is the index of the last unique password to be produced */
		Fixed_Length_Char_Password_Generator(const std::string& rep_string, int first, int last);

		/* rep_string is the pattern string consisting of 'c'\'C'\'d'\'p'\'x'
			data is a vector containing characters to be matched for the 'x' symbols
			rep_indices is a vector with size = the number of appearances of 'x' in rep_string, where each
				of its entries is the index of the data to be matched for this appearance of 'x' 
			first is the index of the first password to be loaded
			last is the index of the last unique password to be produced

			For example: calling with ("xddxx",{0,1,0},{{'#'}, {'0','2','4','6','8'}}, 0, 500) would
						construct a generator for even 3-digit numbers between two '#' */
		Fixed_Length_Char_Password_Generator(const std::string& rep_string, const std::vector<int>& rep_indices,
											 const std::vector<std::vector<char>>& data, int first, int last);

		/* Advances the current password to the next one (if one exists in the range) */
		void advance_password();

		/* Sets the current password to be the one at position (position should be in the range)*/
		void set_password(int position);

		/* Returns whether the generator has more passwords in its range */
		bool has_next();

		/* Reverts to the first password */
		void reset();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length(){
			return true;
		}

		int get_first_position(){
			return first;
		}

		int get_last_position(){
			return last;
		}

		int get_range_length(){
			return last - first;
		}

	private:
		int length;
		int first;
		int last;
		int curr_position;
		std::vector<int> indices;
		std::vector<std::reference_wrapper<std::vector<char>>> char_sources;
		std::vector<std::vector<char>> additional_data; /* use is optional */

		void init(const std::string& rep_string, const std::vector<int>& rep_indices);
};


/*---------------------------------------------------------------------------*/

#endif