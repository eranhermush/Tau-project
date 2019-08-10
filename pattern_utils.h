#ifndef TAU_PROJECT_PATTERN_UTILS_H
#define TAU_PROJECT_PATTERN_UTILS_H

#include <memory>
#include <vector>
#include <cstdint>

/* Functions to filter, process, and extract data from password pattern strings. 
	A patren string should consisit of a the marker characters defined here.
	The tokens are:
		'f' for a file
		'c' for a lowercase english letter
		'C' for an uppercase english letter
		'd' for a digit
		'p' for a punctuation mark
		'{','}' are special characters to represent a set of characters between them
			- if you wish to represnt a set with the char '}' place it in the end ( e.g. {1234}} -> the set [1, 2, 3, 4, }])
			- there is not meaning for a set within a set
*/

namespace Pattern_Utils{

	const char file_marker = 'f';
	const char lowercase_marker = 'c';
	const char uppercase_marker = 'C';
	const char digit_marker = 'd';
	const char punc_marker = 'p';
	const char open_brackets_marker = '{';
	const char close_brackets_marker = '}';

	const unsigned int lowercase_size = 26;
	const unsigned int uppercase_size = 26;
	const unsigned int digit_size = 10;
	const unsigned int punc_size = 32;


	/* Returns a string with the valid parts of the pattern
		Invalid characters are ignored
		Repetitions inside sets are ignores*/
	std::string filter_pattern(const std::string& pattern);

	/*------------- all of the following functions assume a filtered pattern--------------*/

	/* Returns how many files are in the pattern
		The pattern must be filtered */
	unsigned int count_files_in_pattern(const std::string& filtered_pattern);

	/* Returns how many passwords can be produced from the pattern - 
		- When using only the char tokens (files are ignored)
		The pattern must be filtered

		If an overflow happens (when there are too many passwords), UINT64_MAX is returned
			which is indeed an upperbound to the amount of passwords to be produced
		If there are no char tokens 1 is returned (representing the empty string) 
	*/
	uint64_t total_amount_by_char_tokens(const std::string& filtered_pattern);
}



#endif