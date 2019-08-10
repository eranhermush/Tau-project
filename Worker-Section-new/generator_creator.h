#ifndef TAU_PROJECT_GENERATOR_CREATOR_H
#define TAU_PROJECT_GENERATOR_CREATOR_H

#include "password_generator.h"
#include "char_password_generator.h"
#include "file_password_generator.h"
#include "nested_password_generator.h"
#include "../pattern_utils.h"
#include <memory>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <iostream>

namespace Generator_By_Pattern{
	
	/* Creates a password generator accodring to the given pattern
		The pattern is given by the string
			If a token from the char_password_generator.h appears, it is treated as such:
				'c' for a lowercase english letter
				'C' for an uppercase english letter
				'd' for a digit
				'p' for a punctuation mark
			If the character 'f' appears, a file generator is created using the matching path in path
				(if there are not enough paths to fit all 'f' appearances, the function will not create a generator)
			Other characters are ignored
		The generator is initialized according to the range
		
		If the parameters are invalid, or some failure occurs, nullptr is returned

		When a specific file fails, an error message is printed to standard error

		When possible, a nested generator is avoided
	*/
	std::unique_ptr<Password_Generator> create_generator(const std::string& pattern, const std::vector<std::string>& paths,
		uint64_t start, uint64_t end);





	/*-----------More specific functions - No need to use directly-----------*/


	/* The more specific versions used by create_generator
		Unless you have a good reason, no need to use these function just use construtor and avoid pointers	*/

	/* Both create_char_generator and create_nested_generator assume the pattern is already filtered*/
	std::unique_ptr<Char_Pattern_Password_Generator> create_char_generator(const std::string& filtered_pattern,
		uint64_t start, uint64_t end);

	std::unique_ptr<File_Password_Generator> create_file_generator(const std::string& path, uint64_t start, uint64_t end);

	std::unique_ptr<Nested_Password_Generator> create_nested_generator(const std::string& filtered_pattern,
		const std::vector<std::string>& paths, uint64_t start, uint64_t end);

}


#endif 