#include "nested_password_generator.h"

Nested_Password_Generator::Nested_Password_Generator(std::vector<std::unique_ptr<Password_Generator>>& generators, bool should_reset){
	gen_amount = generators.size();
	pass_generators.reserve(gen_amount);
	for(int i = 0; i < gen_amount; ++i){
		pass_generators.push_back(generators[i]->clone());
		if(should_reset){
			pass_generators[i]->reset();
		}
	}

	init();
}

Nested_Password_Generator::Nested_Password_Generator(const Nested_Password_Generator& generator)
	:gen_amount(generator.gen_amount), pass_ranges(generator.pass_ranges), has_next_flag(generator.has_next_flag){
		curr_password = generator.curr_password;
		is_over_flag = generator.is_over_flag;
		pass_generators.reserve(gen_amount);
		for(int i = 0; i < gen_amount; ++i){
			pass_generators.push_back(generator.pass_generators[i]->clone());
		}
}

std::unique_ptr<Password_Generator>  Nested_Password_Generator::clone() const{
	return std::unique_ptr<Password_Generator>(new Nested_Password_Generator(*this));
}


void Nested_Password_Generator::init(){
	int total_length = 0;
	int temp_length = 0;
	std::string temp_password;
	// clear password parameters
	curr_password = "";
	pass_ranges.clear();
	pass_ranges.reserve(gen_amount + 1);
	// construct the first password
	for(int i = 0; i < gen_amount; ++i){
		temp_password = pass_generators[i]->get_password();
		temp_length = temp_password.length();
		// build password
		curr_password += temp_password;
		// update ranges
		pass_ranges.push_back(total_length);
		total_length += temp_length;
		// check if there are more passwords
		if(has_next_flag == false && pass_generators[i]->has_next()){
			has_next_flag = true;
		}
	}
	pass_ranges.push_back(total_length);
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
		if(pass_generators[index]->has_next()){
			// advance this coordinate
			pass_generators[index]->advance_password();
			break;
		}
		// carry the change back
		else{
			pass_generators[index]->reset();
		}
	}
	// change the password
	update_password_suffix(index);

	// update the has next flag, if needed
	for(index = gen_amount - 1; index >= 0; --index){
		if(pass_generators[index]->has_next()){
			// if any sub-generator has another password, the nested generator has another
			return;
		}
	}
	has_next_flag = false;
}

void Nested_Password_Generator::update_password_suffix(int beginning_index){
	std::string sub_password;
	// as long as it is possible do not change the legnth
	int i = beginning_index < gen_amount ? beginning_index : gen_amount; 
	for(; i < gen_amount; ++i){
		sub_password = pass_generators[i]->get_password();
		if(pass_generators[i]->has_fixed_length() || sub_password.length() == (pass_ranges[i+1] - pass_ranges[i])){
			curr_password.replace(pass_ranges[i], pass_ranges[i+1] - pass_ranges[i], sub_password);
		}
		else{
			break;
		}
	}
	// lengths are now different, remove the remaining suffix and build the new string
	curr_password.erase(pass_ranges[i]);
	int total_length = curr_password.length();
	int sub_length = 0;
	for(; i < gen_amount; ++i){
		sub_password = pass_generators[i]->get_password();
		sub_length = sub_password.length();
		curr_password += sub_password;
		pass_ranges[i] = total_length;
		total_length += sub_length;
	}
	pass_ranges[i] = total_length;
}

void Nested_Password_Generator::reset(){
	// zero all generators
	for(int i = 0; i < gen_amount; ++i){
		pass_generators[i]->reset();
	}
	// build the passwords
	init();
}