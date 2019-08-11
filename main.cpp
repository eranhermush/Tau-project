#include "user_text_interface.h"
#include "pattern_utils.h"
#include "aimd_sleep.h"
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "file_manager.h"

int main(int argc, char** argv){
	if(argc < 2){
		std::cerr << "Usage error: expecting 1 argument: a path to the working directory." << std::endl
		return 1;
	}
	std::dir_path(argv[1]);
	std::string func_name;
	std::string func_target;
	std::string func_args_rep;
	uint64_t start = 0, end = 0;
	std::string password_pattern;
	std::vector<std::string> paths;
	unsigned int job_size = 1;
	std::string path;
	bool to_print = true;


	if(User_Text_Interface::get_enumeration_parameters(func_name, func_target, func_args_rep,
		password_pattern, paths, start, end, job_size)){
		return 0;
	}

	file_manager manager(dir_path, password_pattern, paths, func_target, func_name, func_args_rep, start, end, job_size);
	AIMD_Sleep sleeper(2000, 128000, 8000, 2, 16000, 2400);
	bool finish_loop = false;
	while (! finish_loop)
	{
		sleeper.sleep();
		//usleep(10000);
		if(manager.go_over_files(to_print)){
			// useful wake-up
			sleeper.dec();
		}
		else{
			sleeper.inc();
		}
		finish_loop = manager.finish_job();
	}
	return 0;
}