#include "generator_creator.h"

const char data_placeholder_marker = 'x';

std::unique_ptr<Password_Generator> Generator_By_Pattern::create_generator(const std::string& pattern, const std::vector<std::string>& paths,
	uint64_t start, uint64_t end){

	std::string filtered_pattern(Pattern_Utils::filter_pattern(pattern));
	if(filtered_pattern.size() == 0){
		return nullptr;
	}

	unsigned int num_of_files = Pattern_Utils::count_files_in_pattern(filtered_pattern);
	if(num_of_files > paths.size()){
		return nullptr;
	}
	if(num_of_files == 0){
		return Generator_By_Pattern::create_char_generator(filtered_pattern, start, end);
	}
	if(num_of_files == 1 && filtered_pattern.length() == 1){
		// i.e. filtered_pattern is 'f'. The second if statement ensures paths[0] exists
		return Generator_By_Pattern::create_file_generator(paths[0], start, end);
	}
	return Generator_By_Pattern::create_nested_generator(filtered_pattern, paths, start, end);
}




std::unique_ptr<Char_Pattern_Password_Generator> Generator_By_Pattern::create_char_generator(const std::string& filtered_pattern, uint64_t start, uint64_t end){
	std::string final_pattern;
	std::vector<int> data_indices;
	std::vector<std::vector<char>> data;
	std::vector<char> temp_data;
	char temp_char;
	int temp_index = 0;
	bool in_brackets = false;

	for(unsigned int i = 0; i < filtered_pattern.length(); ++i){
		temp_char = filtered_pattern[i];
		if(!in_brackets){
			if(temp_char == Pattern_Utils::open_brackets_marker){
				in_brackets = true;
			}
			else{
				final_pattern.push_back(temp_char);
			}
		}
		else{
			if(temp_char != Pattern_Utils::close_brackets_marker){
				// add char to the current set
				temp_data.push_back(temp_char);
			}
			else{
				if(i+1 < filtered_pattern.length() && filtered_pattern[i+1] == Pattern_Utils::close_brackets_marker){
					// if the last element in the brackets *is* close_bracket_marker, put it
					// in the data and close the bracktes in the next iteration
					temp_data.push_back(temp_char);
				}
				else{
					// close the brackets: push temp_data to the data
					// first sort in order to have a unique representation of each set
					std::sort(temp_data.begin(), temp_data.end());
					// avoid duplicates if possible
					for(temp_index = 0; temp_index < (int) data.size(); ++temp_index){
						if(data[temp_index] == temp_data){
							break;
						}
					}
					if(temp_index == (int) data.size()){
						// temp_data is new
						data.push_back(temp_data);
					}
					// if temp_data was found, push the index. If it is new, temp_index is its newly defined index
					data_indices.push_back(temp_index);

					temp_data.clear();
					final_pattern.push_back(data_placeholder_marker);
					in_brackets = false;
				}
			}
		}
	}

	if(data_indices.size() == 0){
		return std::unique_ptr<Char_Pattern_Password_Generator>(new Char_Pattern_Password_Generator(final_pattern, start, end));
	}
	return std::unique_ptr<Char_Pattern_Password_Generator>(new Char_Pattern_Password_Generator(final_pattern, data_indices, data, start, end));
}



std::unique_ptr<File_Password_Generator> Generator_By_Pattern::create_file_generator(const std::string& path, uint64_t start, uint64_t end){
	std::unique_ptr<File_Password_Generator> fpg_ptr(new File_Password_Generator(path, start, end));
	if(fpg_ptr->file_error()){
		std::cerr << "Could not load file in " << path << std::endl;
		return nullptr;
	}
	return fpg_ptr;
}


std::unique_ptr<Nested_Password_Generator> Generator_By_Pattern::create_nested_generator(const std::string& filtered_pattern,
	const std::vector<std::string>& paths, uint64_t start, uint64_t end){

	unsigned int file_count = 0;
	char temp_char;
	bool in_brackets = false;
	std::string temp_sub_pattern;
	std::unique_ptr<Password_Generator> temp_generator;
	std::vector<std::unique_ptr<Password_Generator>> inner_generators;

	for(unsigned int i = 0; i < filtered_pattern.length(); ++i){
		temp_char = filtered_pattern[i];
		if(!in_brackets && temp_char == Pattern_Utils::file_marker){
			if(temp_sub_pattern.length() > 0){
				// if a file-free sub pattern was aggrigated, create a char generator
				temp_generator = Generator_By_Pattern::create_char_generator(temp_sub_pattern, 0, 0);
				inner_generators.push_back(std::move(temp_generator));
				temp_sub_pattern.clear();
			}
			if(file_count < paths.size()){
				temp_generator = Generator_By_Pattern::create_file_generator(paths[file_count], 0, 0);
				if(temp_generator == nullptr){
					return nullptr;
				}
				inner_generators.push_back(std::move(temp_generator));
				++file_count;
			}
		}
		else{
			temp_sub_pattern.push_back(temp_char);
			if(temp_char == Pattern_Utils::close_brackets_marker){
				// If the final element in the bracket is close_brackets_marker
				// Then the next character is not a file_marker or open_brackets_marker, so no need to check
				in_brackets = false;
			}
			else if(temp_char == Pattern_Utils::open_brackets_marker){
				in_brackets = true;
			}
		}
	}
	if(temp_sub_pattern.length() > 0){
		// if a file-free sub pattern was aggrigated, create a char generator
		temp_generator = Generator_By_Pattern::create_char_generator(temp_sub_pattern, 0, 0);
		inner_generators.push_back(std::move(temp_generator));
		temp_sub_pattern.clear();
	}

	return std::unique_ptr<Nested_Password_Generator>(new Nested_Password_Generator(inner_generators, start, end));
}