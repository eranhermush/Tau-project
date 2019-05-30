#include "char_password_generator.h"

Char_Pattern_Password_Generator::Char_Pattern_Password_Generator(const std::string& rep_string, int first, int last)
	:first(first), last(last){
		init(rep_string, std::vector<int>());
}

Char_Pattern_Password_Generator::Char_Pattern_Password_Generator(const std::string& rep_string, const std::vector<int>& rep_indices,
 const std::vector<std::vector<char>>& data, int first, int last) 
:first(first), last(last), additional_data(data){
	init(rep_string, rep_indices);
}

Char_Pattern_Password_Generator::Char_Pattern_Password_Generator(const Char_Pattern_Password_Generator& generator)
	:length(generator.length), first(generator.first), last(generator.last), curr_position(generator.curr_position),
	indices(generator.indices), sources_index(generator.sources_index), additional_data(generator.additional_data){

		curr_password = generator.curr_password;
		is_over_flag = generator.is_over_flag;
		char_sources.reserve(curr_password.length());
		std::vector<int>::const_iterator it = sources_index.begin();
		for(; it != sources_index.end(); ++it){
			if(*it >= 0){
				/* This character's source is in the additionl data */
				char_sources.push_back(std::ref(additional_data[*it]));
			}
			else{
				/* This character's source is in the default options */
				switch(*it){
					case -'c':
						char_sources.push_back(std::ref(DATA_LOWERCASE));
						break;
					case -'C':
						char_sources.push_back(std::ref(DATA_UPPERCASE));
						break;
					case -'d':
						char_sources.push_back(std::ref(DATA_DIGITS));
						break;
					case -'p':
						char_sources.push_back(std::ref(DATA_PUNCTUATION));
						break;
				}
			}
		}
}

void Char_Pattern_Password_Generator::init(const std::string& rep_string, const std::vector<int>& rep_indices){
	length = 0;
	int x_count = 0;
	int x_index = 0;

	sources_index.reserve(rep_string.length());
	char_sources.reserve(rep_string.length());

	std::string::const_iterator it;
	for(it = rep_string.begin(); it != rep_string.end(); ++it){
		++length;
		switch (*it){
			case 'c':
				char_sources.push_back(std::ref(DATA_LOWERCASE));
				sources_index.push_back(-'c');
				break;
			case 'C':
				char_sources.push_back(std::ref(DATA_UPPERCASE));
				sources_index.push_back(-'C');
				break;
			case 'd':
				char_sources.push_back(std::ref(DATA_DIGITS));
				sources_index.push_back(-'d');
				break;
			case 'p':
				char_sources.push_back(std::ref(DATA_PUNCTUATION));
				sources_index.push_back(-'p');
				break;
			case 'x':
				// check for the related data
				if(x_count < rep_indices.size()){
					x_index = rep_indices[x_count];
					if(x_index < additional_data.size()){
						char_sources.push_back(std::ref(additional_data[x_index]));
						sources_index.push_back(x_index);
						++x_count;
						break;
					}
				}
				// if we failed to match x, then ignore it
			default:
				--length;
		}
	}
	// set to start
	indices = std::vector<int>(length, 0);
	curr_password = std::string(length, '\0');
	reset();
}


std::unique_ptr<Password_Generator>  Char_Pattern_Password_Generator::clone() const{
	return std::unique_ptr<Password_Generator>(new Char_Pattern_Password_Generator(*this));
}


void Char_Pattern_Password_Generator::advance_password(){
	if(!has_next()){
		// finished with the last password
		is_over_flag = true;
		return;
	}

	int sub_index;
	for(int i = length-1; i >= 0; --i){
		sub_index = ++indices[i];
		// if is a valid index, use it and break
		if(sub_index < char_sources[i].get().size()){
			curr_password[i] = char_sources[i].get()[sub_index];
			break;
		}
		// otherwise, carry the change forward
		else{
			indices[i] = 0;
			curr_password[i] = char_sources[i].get()[0];
		}
	}
	++curr_position;
}

void Char_Pattern_Password_Generator::set_password(int position){
	int sub_length = 0, sub_index = 0;
	// match each charcter from right (Least Significant) to left (Most Significant)
	for(int i = length-1; i >= 0; --i){
		sub_length = char_sources[i].get().size();
		sub_index = position % sub_length;
		indices[i] = sub_index;
		curr_password[i] = char_sources[i].get()[sub_index];
		position /= sub_length;
	}
	curr_position = position;
}

bool Char_Pattern_Password_Generator::has_next() const{
	return curr_position < last;
}

void Char_Pattern_Password_Generator::reset(){
	set_password(first);
}