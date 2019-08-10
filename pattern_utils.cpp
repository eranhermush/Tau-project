#include "pattern_utils.h"

std::string Pattern_Utils::filter_pattern(const std::string& pattern){
	std::string result;
	std::string temp_in_brackets;
	char temp_char;
	unsigned int len = pattern.size();
	bool opened_brackets = false;

	for(unsigned int i = 0; i < len; ++i){
		temp_char = pattern[i];
		if(!opened_brackets){
			if(temp_char == Pattern_Utils::file_marker || temp_char == Pattern_Utils::lowercase_marker || temp_char == Pattern_Utils::uppercase_marker
				|| temp_char == Pattern_Utils::digit_marker || temp_char == Pattern_Utils::punc_marker){
				// this is a valid normal char
				result.push_back(temp_char);
				continue;
			}
			else if(temp_char == Pattern_Utils::open_brackets_marker){
				temp_in_brackets.push_back(temp_char);
				opened_brackets = true;
				continue;
			}
		}
		else{
			if(temp_char != Pattern_Utils::close_brackets_marker){
				// put in the brackets, but avoid multiplicities
				// starting at 1 to avoid the open brackets marker
				if(temp_in_brackets.find_first_of(temp_char, 1) == std::string::npos){
					temp_in_brackets.push_back(temp_char);
				}
			}
			else{
				// if there are 2 adjacent close_brakets, it means the close_brakets marker is in the barkets
				if(i+1 < len && pattern[i+1] == Pattern_Utils::close_brackets_marker){
					temp_in_brackets.push_back(temp_char);
					++i; // another character was consumed
				}
				temp_in_brackets.push_back(temp_char);
				//no need for empty brackets
				if(temp_in_brackets.length() > 2){
					result.append(temp_in_brackets);
				}
				temp_in_brackets.clear();
				opened_brackets = false;
			}
		}
	}
	return result;
}



unsigned int Pattern_Utils::count_files_in_pattern(const std::string& filtered_pattern){
	unsigned int count = 0;
	bool in_brackets = false;
	char temp_char;
	for(unsigned int i = 0; i < filtered_pattern.length(); ++i){
		temp_char = filtered_pattern[i];
		if(in_brackets){
			// even if the brackets contain close_brackets_marker and they
			// are not technically over, the next char is close_brackets_marker
			// which is not file_marker
			if(temp_char == Pattern_Utils::close_brackets_marker){
				in_brackets = false;
			}
		}
		else{
			if(temp_char == Pattern_Utils::file_marker){
				++count;
			}
			else if(temp_char == Pattern_Utils::open_brackets_marker){
				in_brackets = true;
			}
		}
	}
	return count;
}


uint64_t Pattern_Utils::total_amount_by_char_tokens(const std::string& filtered_pattern){
	uint64_t total = 1;
	uint64_t prev_total = 1;
	unsigned int temp_brackets_size = 0;
	unsigned int temp_size = 1;
	bool in_brackets = false;
	char temp_char;

	for(unsigned int i = 0; i < filtered_pattern.length(); ++i){
		temp_char = filtered_pattern[i];
		if(in_brackets){
			if(temp_char == Pattern_Utils::close_brackets_marker){
				if(i+1 < filtered_pattern.length() && filtered_pattern[i+1] == Pattern_Utils::close_brackets_marker){
					// the first close_brackets_marker is in the brackets
					++temp_brackets_size;
					++i; // another character was consumed
				}
				if(temp_brackets_size > 0){
					prev_total = total;
					total *= temp_brackets_size;
					if(total / temp_brackets_size != prev_total){
						// overflow
						return UINT64_MAX;
					}
				}
				temp_brackets_size = 0;
				in_brackets = false;
			}
			else{
				++temp_brackets_size;
			}
		}
		else{
			if(temp_char == Pattern_Utils::open_brackets_marker){
				in_brackets = true;
				continue;
			}
			else if(temp_char == Pattern_Utils::lowercase_marker){
				temp_size = Pattern_Utils::lowercase_size;
			}
			else if(temp_char == Pattern_Utils::uppercase_marker){
				temp_size = Pattern_Utils::uppercase_size;
			}
			else if(temp_char == Pattern_Utils::digit_marker){
				temp_size = Pattern_Utils::digit_size;
			}
			else if(temp_char == Pattern_Utils::punc_marker){
				temp_size = Pattern_Utils::punc_size;
			}
			prev_total = total;
			total *= temp_size;
			if(total / temp_size != prev_total){
				// overflow
				return UINT64_MAX;
			}
			temp_size = 1;
		}
	}
	return total;
}