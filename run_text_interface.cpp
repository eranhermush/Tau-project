#include "user_text_interface.h"
#include "pattern_utils.h"
#include <string>
#include <vector>
#include <iostream>

int main(){
	std::string func_name;
	std::string func_target;
	std::string func_args_rep;
	uint64_t start = 0, end = 0;
	std::string password_pattern;
	std::vector<std::string> paths;
	unsigned int job_size = 1;


	if(User_Text_Interface::get_enumeration_parameters(func_name, func_target, func_args_rep,
		password_pattern, paths, start, end, job_size)){
		return 1;
	}

	std::cout << "---------printing params-------------" << std::endl;
	std::cout << "function: " << func_name << std::endl;
	std::cout << "target: " << func_target << std::endl;
	std::cout << "func_args: " << func_args_rep << std::endl;
	std::cout << "pattern: " << password_pattern << std::endl;
	for(unsigned int i = 0; i < paths.size(); ++i){
		std::cout << "file" << i+1 << ": " << paths[i] << std::endl;
	}
	std::cout << "start: " << start << "\tend: " << end << std::endl;
	std::cout << "Job size per worker: " << job_size << std::endl; 
	return 0;
}