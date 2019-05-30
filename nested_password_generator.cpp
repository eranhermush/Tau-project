#include "nested_password_generator.h"

Nested_Password_Generator::Nested_Password_Generator(std::vector<Password_Generator>& generators, bool should_reset=true){
	gen_amount = generators.size();
	pass_generators.reserve(gen_amount);
	for(int i = 0; i < gen_amount; ++i){
		pass_generators.push_back(generators[i].clone())
		if(should_reset){
			pass_generators[i]->reset();
		}
	}

	init();
}

void Nested_Password_Generator::init(){
	int total_length = 0;
	int temp_length = 0;
	std::string& temp_password;
	// clear password parameters
	curr_password = "";
	pass_ranges.clear();
	pass_ranges.reserve(gen_amount);
	// construct the first password
	for(int i = 0; i < gen_amount; ++i){
		temp_password = *pass_generators[i].get_password();
		temp_length = temp_password.length();
		// build password
		curr_password += temp_password;
		// update ranges
		pass_ranges.push_back(std::array<int,2>({total_length, total_length + temp_length}));
		total_length += temp_length;
		// check if there are more passwords
		if(has_next_flag == false && *pass_generators[i].has_next()){
			has_next_flag = true;
		}
	}
}

void Nested_Password_Generator::advance_password(){
	if(!has_next()){
		// finished with last password
		is_over_flag = true;
		return;
	}
	int index;
	// advance lexicographically, change only suffixes
	// find the index in which the suffix begins and set the generators accordingly
	for(index = gen_amount - 1; index >= 0; --index){
		if(*pass_generators[index].has_next()){
			// advance this coordinate
			*pass_generators[index].advance_password();
			break;
		}
		// carry the change back
		else{
			*pass_generators[index].reset();
		}
	}

	// change the password
	update_paswword_suffix(index);

	// update the has next flag, if needed
	for(int i = gen_amount - 1; i >= 0; --i){
		if(*pass_generators[index].has_next()){
			// if any sub-generator has another password, the nested generator has another
			return;
		}
	}
	has_next_flag = false;
}

void Nested_Password_Generator::update_paswword_suffix(int beginning_index){
	std::string& sub_password;
	// as long as it is possible do not change the legnth
	int i = beginning_index; 
	for(; i < gen_amount; ++i){
		sub_password = *pass_generators[i].get_password();
		if(*pass_generators[i].has_fixed_length() || sub_password.legnth() == (pass_ranges[i][1] - pass_ranges[i][0])){

		}
	}
}

void Nested_Password_Generator::reset(){
	// zero all generators
	for(int i = 0; i < gen_amount; ++i){
		pass_generators[i]->reset();
	}
	// build the passwords
	init();
}