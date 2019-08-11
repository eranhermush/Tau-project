#include "user_text_interface.h"

/*--------------Private small utility function------------*/

/* Returns the string represents a positive number */
bool check_number(const std::string& number);

/* Returns the string is made up of hexadecimal digits */
bool check_hex(const std::string& hex);

/* Returns the group number if group name is one of the SAE groups,
	otherwise, returns 0 */
int get_group_number(const std::string& group_name);

/* Pack the argumets to a string by concating with format:
	0(l2)(l1)(s) where l1 = length(l2); l2 = length(s)
	and s is a string. Or the string "-1" on empty args */
std::string pack_args(const std::vector<std::string>& args);

/* Returns min(a*b, 2^64-1) */
uint64_t overflow_safe_mul(uint64_t a, uint64_t b);

/* Returna an estimate of a good job size per worker for the parameters */
unsigned int estimate_good_job_size(uint64_t workload, unsigned int num_of_files, const std::string& func_name);


bool check_number(const std::string& number){
	for(unsigned int i = 0; i < number.size(); ++i){
		if('0' <= number[i] && number[i] <= '9'){
			continue;
		}
		return false;
	}
	return true;
}

bool check_hex(const std::string& hex){
	for(unsigned int i = 0; i < hex.size(); ++i){
		if('0' <= hex[i] && hex[i] <= '9'){
			continue;
		}
		if('a' <= hex[i] && hex[i] <= 'f'){
			continue;
		}
		if('A' <= hex[i] && hex[i] <= 'F'){
			continue;
		}
		return false;
	}
	return true;
}

int get_group_number(const std::string& group_name){
	std::string lc_name, str;
	lc_name.resize(group_name.size());
	std::transform(group_name.begin(), group_name.end(), lc_name.begin(), [](unsigned char c){ return std::tolower(c); });
	for(unsigned int g = 19; g <= 24; ++g){
		str = std::to_string(g);
		if(lc_name == str || lc_name == "group"+str || lc_name == "group "+str){
			return g;
		}
	}
	return 0;
}

std::string pack_args(const std::vector<std::string>& args){
	if(args.size() == 0){
		return "-1";
	}
	std::string result;
	std::string len_str;
	std::string lenlen_str;
	for(unsigned int i = 0; i < args.size(); ++i){
		len_str = std::to_string(args[i].length());
		lenlen_str = std::to_string(len_str.length());
		result += "0" + lenlen_str + len_str + args[i];
	}
	return result;
}

uint64_t overflow_safe_mul(uint64_t a, uint64_t b){
	if(a == 0 || b == 0){
		return 0;
	}
	uint64_t mul = a*b;
	if(mul/b != a){
		//overflow
		return UINT64_MAX;
	}
	return mul;
}

unsigned int estimate_good_job_size(uint64_t workload, unsigned int num_of_files, const std::string& func_name){
	unsigned int base_size = 250000;
	if(func_name == "sae ecc" || func_name == "sae ffc"){
		base_size = 100000;
	}
	if(workload < 1000000){
		base_size *= 4;
		base_size /= 5;
	}
	if(num_of_files == 0){
		return base_size;
	}
	unsigned int file_factor = num_of_files >= 3 ? 12 : 6;
	return file_factor * base_size;

}


bool User_Text_Interface::get_enumeration_parameters(std::string& func_name, std::string& func_target, std::string& func_args_rep_string,
	std::string& password_pattern, std::vector<std::string>& paths, uint64_t& start, uint64_t& end, unsigned int& job_size){

	bool should_exit = false;
	std::vector<std::string> func_args;
	paths.empty();
	std::string path;
	uint64_t file_size = 1;
	uint64_t total_size = 1;

	begining_prompt();
	// function
	func_name = get_function_name(should_exit);
	if(should_exit){
		return true;
	}
	User_Text_Interface::get_function_paramaters(func_name, should_exit, func_target, func_args);
	if(should_exit){
		return true;
	}
	func_args_rep_string = pack_args(func_args);
	// pattern
	password_pattern = get_password_pattern(should_exit);
	if(should_exit){
		return true;
	}
	unsigned int file_amount = Pattern_Utils::count_files_in_pattern(password_pattern);
	for(unsigned int i = 0; i < file_amount; ++i){
		paths.push_back(User_Text_Interface::get_file_path(file_amount == 1 ? -1 : (int) i+1, should_exit, file_size));
		if(should_exit){
			return true;
		}
		total_size = overflow_safe_mul(total_size, file_size);
	}
	total_size = overflow_safe_mul(total_size, Pattern_Utils::total_amount_by_char_tokens(password_pattern));
	get_enumeration_bounds(should_exit, start, end, total_size);
	if(should_exit){
		return true;
	}
	//job size
	job_size = estimate_good_job_size(end - start + 1, paths.size(), func_name);
	please_wait_prompt();
	return false;
}










void User_Text_Interface::begining_prompt(){
	std::cout << BEGIN_PROMPT << std::endl;
}

void User_Text_Interface::please_wait_prompt(){
	std::cout << WAIT_MSG << std::endl;
}



bool User_Text_Interface::check_for_help_or_exit(const std::string& reply, const std::string& help_reply, bool& should_exit){
	if(reply == EXIT_REQUEST){
		should_exit = true;
		return true;
	}
	if(reply == HELP_REQUEST){
		std::cout << help_reply << std::endl;
		return true;
	}
	return false;
}


std::string User_Text_Interface::get_function_name(bool& should_exit){
	std::string reply;
	std::string lc_name;
	bool answered = false;
	std::cout << "Please enter the name of the function you would like to inverse:" << std::endl;
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, reply);
		if(check_for_help_or_exit(reply, FUNCTION_HELP, should_exit)){
			if(should_exit){
				return "";
			}
			continue;
		}
		lc_name.resize(reply.size());
		std::transform(reply.begin(), reply.end(), lc_name.begin(), [](unsigned char c){ return std::tolower(c); });
		if(FUNCTION_NAMES.find(lc_name) == FUNCTION_NAMES.end()){
			std::cout << reply << " is not supported. Please try again." << std::endl;
		}
		else{
			answered = true;
		}
	}
	return lc_name;
}


std::string User_Text_Interface::get_password_pattern(bool& should_exit){
	std::string reply;
	std::string filtered_pattern;
	bool answered = false;
	bool answered_check = false;
	std::cout << "Please enter the password pattern:" << std::endl;
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, reply);
		if(check_for_help_or_exit(reply, PATTERN_HELP, should_exit)){
			if(should_exit){
				return "";
			}
			continue;
		}
		filtered_pattern = Pattern_Utils::filter_pattern(reply);
		if(filtered_pattern.size() == 0){
			std::cout << "Pattern can not be empty." << std::endl;
			continue;
		}
		if(filtered_pattern != reply){
			std::cout << "Did you mean \'" << filtered_pattern << "\' ?(y/n)" << std::endl;
			answered_check = false;
			while(!answered_check){
				std::cout << PROMPT_REPLY;
				std::getline(std::cin, reply);
				if(reply == EXIT_REQUEST){
					should_exit = true;
					return "";
				}
				else if(reply.size() == 0){
					continue;
				}
				else if(reply[0] == 'y'){
					answered_check = true;
					answered = true;
				}
				else if(reply[0] == 'n'){
					answered_check = true;
					answered = false;
					// repeat prompt for do over
					std::cout << "Please enter the password pattern:" << std::endl;
				}
			}
		}
		else{
			answered = true;
		}

	}
	return filtered_pattern;
}



std::string User_Text_Interface::get_file_path(int file_number, bool& should_exit, uint64_t& size, bool test_size, bool absolute_path){
	std::string path;
	bool answered = false;
	if(file_number >= 0){
		std::cout << "Please enter the path to file #" << file_number << ":" << std::endl;
	}
	else{
		std::cout << "Please enter the path to the file:" << std::endl;
	}
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, path);
		if(check_for_help_or_exit(path, NO_HELP, should_exit)){
			if(should_exit){
				return "";
			}
			continue;
		}
		if(absolute_path){
			path = helpful_functions::get_absolute_path(path);
			if(path.size() == 0){
				std::cout << "Invalid file. Please try again." << std::endl;
				continue;
			}
		}
		if(test_size){
			size = helpful_functions::get_file_size(path);
			if(size == 0){
				std::cout << "Invalid file. Please try again." << std::endl;
				continue;
			}
		}
		answered = true;
	}
	return path;
}


void User_Text_Interface::get_enumeration_bounds(bool& should_exit, uint64_t& start, uint64_t& end, uint64_t total_upper_bound){
	std::string reply;
	std::stringstream reply_stream;
	std::cout << "Please enter the enumeration bounds (between 0 and " << total_upper_bound << ") or \"all\":" << std::endl;
	bool answered = false;
	bool got_start = false;
	uint64_t val = 0;
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, reply);
		if(check_for_help_or_exit(reply, BOUNDS_HELP, should_exit)){
			if(should_exit){
				return;
			}
			continue;
		}
		if(reply == "all" || reply == "ALL" || reply == "All"){
			end = total_upper_bound - 1;
			start = 0;
			return;
		}
		if(!check_number(reply)){
			std::cout << "Invalid input. Please try again" << std::endl;
		}
		else{
			reply_stream.clear();
			reply_stream.str(reply);
			reply_stream >> val;
		}
		if(val >= total_upper_bound){
			std::cout << "Invalid bound. Must be less than " << total_upper_bound << ". Please try again." << std::endl;
			continue;
		}
		else{
			if(!got_start){
				start = val;
				got_start = true;
			}
			else{
				if(start > val){
					std::cout << "Invalid upper bound. Must be no less than " << start << ". Please try again." << std::endl;
				}
				else{
					end = val;
					answered = true;
				}
			}
		}
	}

}




void User_Text_Interface::get_function_paramaters(const std::string& func_name, bool& should_exit, std::string& target, std::vector<std::string>& args){
	if(func_name == "sae ecc" || func_name == "sae ffc"){
		User_Text_Interface::get_sampling_parameters(func_name, should_exit, target, args);
	}
	else{
		User_Text_Interface::get_hash_parametes(func_name, should_exit, target, args);
	}
}

void User_Text_Interface::get_hash_parametes(const std::string& func_name, bool& should_exit, std::string& target, std::vector<std::string>& args){
	std::unordered_map<std::string, unsigned int>::const_iterator it = DIGEST_SIZES.find(func_name);
	if(it == DIGEST_SIZES.end()){
		std::cout << "An error occured." << std::endl;
		should_exit = true;
		return;
	}
	unsigned int digest_size = it->second;
	bool answered = false;
	bool answered_check = false;
	std::string reply;
	args.clear();
	std::cout << "Please enter the hash value:" << std::endl;
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, reply);
		if(check_for_help_or_exit(reply, HASH_HELP, should_exit)){
			if(should_exit){
				return;
			}
			continue;
		}
		if(!check_hex(reply)){
			std::cout << "Invalid format. Please try again." << std::endl;
			continue;
		}
		else{
			target = reply;
			if(reply.size()*4 != digest_size){
				std::cout << "Expected " << digest_size << " bits, but received " << reply.size()*4 << ". Are you sure?(y/n)" <<std::endl;
				answered_check = false;
				while(!answered_check){
					std::cout << PROMPT_REPLY;
					std::getline(std::cin, reply);
					if(reply == EXIT_REQUEST){
						should_exit = true;
						return;
					}
					else if(reply.size() == 0){
						continue;
					}
					else if(reply[0] == 'y'){
						answered_check = true;
						answered = true;
					}
					else if(reply[0] == 'n'){
						answered_check = true;
						answered = false;
						// repeat prompt for do over
						std::cout << "Please enter the hash value:" << std::endl;
					}
				}
			}
			else{
				answered = true;
			}
		}
	}
}

void User_Text_Interface::get_sampling_parameters(const std::string& protocol, bool& should_exit, std::string& path, std::vector<std::string>& args){
	std::string reply;
	bool answered = false;
	bool ffc = (protocol == "sae ffc");
	args.clear();
	uint64_t filesize = 0;
	int group_number;
	
	std::cout << "Please enter the path to the samples file:" << std::endl;
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, reply);
		if(check_for_help_or_exit(reply, NO_HELP, should_exit)){
			if(should_exit){
				return;
			}
			continue;
		}
		path = helpful_functions::get_absolute_path(reply);
		if(path.size() == 0){
			std::cout << "Invalid samples file. Please try again." << std::endl;
			continue;
		}
		filesize = helpful_functions::get_file_size(path);
		if(filesize == 0){
			std::cout << "Invalid samples file. Please try again." << std::endl;
			continue;
		}
		answered = true;
	}

	answered = false;
	std::cout << "Please enter the group name:" << std::endl;
	while(!answered){
		std::cout << PROMPT_REPLY;
		std::getline(std::cin, reply);
		if(ffc){
			if(check_for_help_or_exit(reply, FFC_HELP, should_exit)){
				if(should_exit){
					return;
				}
				continue;
			}
		}
		else{
			if(check_for_help_or_exit(reply, ECC_HELP, should_exit)){
				if(should_exit){
					return;
				}
				continue;
			}
		}
		group_number = get_group_number(reply);
		if((ffc && (22 <= group_number && group_number <= 24)) || (!ffc && 19 <= group_number && group_number <= 21)){
			args.push_back("group" + std::to_string(group_number));
			answered = true;
		}
		else{
			std::cout << "Invalid group name. Please try again." << std::endl;
		}
	}
}