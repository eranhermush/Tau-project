#include "nested_password_generator.h"

Nested_Password_Generator::Nested_Password_Generator(const std::vector<std::unique_ptr<Password_Generator>>& generators,
	const std::vector<uint64_t>& start_pos, const std::vector<uint64_t>& end_pos){

	gen_amount = std::min(generators.size(), std::min(start_pos.size(), end_pos.size()));
	init_generators(generators);

	curr_pos_vector.reserve(gen_amount);

	init_vectors(start_pos, end_pos);

	curr_pos_vector = start_pos_vector;

	total_start_pos = vector_to_number(start_pos_vector, generator_lengths);
	total_end_pos = vector_to_number(start_pos_vector, generator_lengths);


	init_password();
}

Nested_Password_Generator::Nested_Password_Generator(const std::vector<std::unique_ptr<Password_Generator>>& generators,
	uint64_t total_start, uint64_t total_end){

	gen_amount = generators.size();
	init_generators(generators);

	curr_pos_vector.reserve(gen_amount);
	
	std::vector<uint64_t> sp(gen_amount, 0);
	std::vector<uint64_t> ep(gen_amount, 0);
	number_to_vector(total_start, generator_lengths, sp);
	number_to_vector(total_end, generator_lengths, ep);

	init_vectors(sp, ep);

	curr_pos_vector = start_pos_vector;

	total_start_pos = vector_to_number(start_pos_vector, generator_lengths);
	total_end_pos = vector_to_number(start_pos_vector, generator_lengths);


	init_password();
}

Nested_Password_Generator::Nested_Password_Generator(const Nested_Password_Generator& generator)
	:gen_amount(generator.gen_amount){
		init_generators(generator.pass_generators);
		init_vectors(generator.start_pos_vector, generator.end_pos_vector);
		if(generator.curr_pos_vector < start_pos_vector){
			curr_pos_vector = start_pos_vector;
		}
		else if(generator.curr_pos_vector > end_pos_vector){
			curr_pos_vector = end_pos_vector;
		}
		else{
			curr_pos_vector = generator.curr_pos_vector;
		}
		has_next_flag = (curr_pos_vector < end_pos_vector);
		init_password();
}

std::unique_ptr<Password_Generator>  Nested_Password_Generator::clone() const{
	return std::unique_ptr<Password_Generator>(new Nested_Password_Generator(*this));
}

uint64_t Nested_Password_Generator::vector_to_number(const std::vector<uint64_t>& digits, const std::vector<uint64_t>& bases){
	int length = std::min(bases.size(), digits.size());
	uint64_t result = 0;
	uint64_t prev_base = 1;
	for(int i = 0; i < length; ++i){
		result = result * prev_base + digits[i];
		prev_base = bases[i];
	}
	return result;
}

void Nested_Password_Generator::number_to_vector(uint64_t number, const std::vector<uint64_t>& bases, std::vector<uint64_t>& dest){
	int length = std::min(bases.size(), dest.size());
	for(int i = length - 1; i >= 0; --i){
		dest[i] = number % bases[i];
		number /= bases[i];
	}
}

void Nested_Password_Generator::init_generators(const std::vector<std::unique_ptr<Password_Generator>>& generators){
	pass_generators.reserve(gen_amount);
	generator_lengths.reserve(gen_amount);

	for(int i = 0; i < gen_amount; ++i){
		pass_generators.push_back(generators[i]->clone());
		pass_generators[i]->expand_to_max_range();
		generator_lengths.push_back(pass_generators[i]->get_range_length());
	}
}

void Nested_Password_Generator::init_vectors(const std::vector<uint64_t>& start_pos, const std::vector<uint64_t>& end_pos){
	
	end_pos_vector.reserve(gen_amount);
	start_pos_vector.reserve(gen_amount);

	for(int i = 0; i < gen_amount; ++i){
		start_pos_vector.push_back(std::min(start_pos[i], generator_lengths[i] - 1));
		end_pos_vector.push_back(std::min(end_pos[i], generator_lengths[i] - 1));
	}
	// vector comare is lexicographic compare
	if(start_pos_vector > end_pos_vector){
		end_pos_vector = start_pos_vector;
	}
}

void Nested_Password_Generator::set_password(const std::vector<uint64_t>& new_pos){
	if(new_pos < start_pos_vector || new_pos > end_pos_vector){
		return;
	}
	curr_pos_vector = new_pos;
	init_password();
	is_over_flag = false;
	if(!has_next_flag && curr_pos_vector < end_pos_vector){
		has_next_flag = true;
	}
}


void Nested_Password_Generator::init_password(){
	//init the generators
	for(int i = 0; i < gen_amount; ++i){
		pass_generators[i]->set_password(curr_pos_vector[i]);
	}
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
			curr_pos_vector[index] = pass_generators[index]->get_curr_position();
			break;
		}
		// carry the change back
		else{
			pass_generators[index]->reset();
			curr_pos_vector[index] = pass_generators[index]->get_curr_position();
		}
	}
	// if we 'carried' from the first generator, there is no next at all
	// this might happen if the first generator only had the one password
	if(index < 0){
		has_next_flag = false;
		is_over_flag = true;
		return;
	}
	// change the password
	update_password_suffix(index);

	// update the has next flag, if we reached the last vector there is no next in the range
	// may not be accurate when it comes to actual end files, that's why there are more tests
	has_next_flag = (curr_pos_vector < end_pos_vector);
}

void Nested_Password_Generator::update_password_suffix(int beginning_index){
	std::string sub_password;
	// as long as it is possible do not change the legnth
	int i = beginning_index < gen_amount ? beginning_index : gen_amount; 
	for(; i < gen_amount; ++i){
		sub_password = pass_generators[i]->get_password();
		if(pass_generators[i]->has_fixed_length() || sub_password.length() == (unsigned int)(pass_ranges[i+1] - pass_ranges[i])){
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
	// zero all coordinates
	set_password(start_pos_vector);
}

void Nested_Password_Generator::expand_to_max_range(){
	uint64_t total_size = 1;
	for(int i = 0; i < gen_amount; ++i){
		start_pos_vector[i] = 0;
		end_pos_vector[i] = generator_lengths[i] - 1;
		total_size *= generator_lengths[i];
	}
	if(!has_next_flag && curr_pos_vector < end_pos_vector){
		has_next_flag = true;
	}
	if(is_over_flag && curr_pos_vector != end_pos_vector){
		is_over_flag = false;
	}
	total_start_pos = 0;
	total_end_pos = total_size - 1;
}