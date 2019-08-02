#include "preimage_seeker.h"

Preimage_Seeker::Preimage_Seeker(const Password_Generator& gen, const Preimage_Matcher& match, int id, std::string& dir_path, int sleep, bool should_reset)
	:generator(gen.clone()), matcher(match.clone()){
	if(should_reset){
		generator->reset();
	}
	this->id = id;
	this->dir_path = dir_path;
	this->microseconds_sleep = sleep;
}

std::vector<std::string> Preimage_Seeker::seek_all(){
	std::vector<std::string> answer;

	while(!generator->is_over()){
		if(matcher->test(generator->get_password())){
			answer.push_back(generator->get_password());
		}
		generator->advance_password();
	}

	return answer;
}

std::string Preimage_Seeker::seek_next(bool& found){
	found = false;
	std::string answer;

	while(!generator->is_over() && !found){
		if(matcher->test(generator->get_password())){
			found = true;
			answer = generator->get_password();
		}
		generator->advance_password();
	}

	return answer;
}


bool Preimage_Seeker::start()
{
	bool ret_val = false;
	ret_val =  helpful_functions::write_data_to_file(this->dir_path, std::to_string(this->id), "3"); // 3 indicates that we start a new worker
	if (! ret_val)
	{
		return false;
	}
	ret_val = get_job(3, false);
	if (! ret_val)
	{
		return false;
	}
	return true;
}

bool Preimage_Seeker::get_job(int status_start, bool to_write)
{
	bool ret_val = false;
	bool finish = false;
	file_object fileob;
	int retVal = 0;
	std::string path =  this->dir_path + "/" + std::to_string(this->id) + ".txt";


	if(to_write)
	{
		ret_val =  helpful_functions::write_data_to_file(this->dir_path, std::to_string(this->id), std::to_string(status_start)); 
		if (! ret_val)
		{
			return false;
		}		
	}
	// waits for a job
	/*
	* The loop:
	*	sleep
	* 	check the file
	*/
	while (! finish)
	{
		usleep(this->microseconds_sleep);
		retVal = helpful_functions::file_to_file_object(fileob, path, true);
		if (retVal == -1){
            return false;
        }
        // we need status 

        // there are no more works, or gets a work
	    if (fileob.get_status() == 5 || fileob.get_status() == 0) 
	    {
	    	this->file_obj = fileob;
	    	finish = true;
	        return true;
	    }

	}
	return true;

}

bool set_job(int status, std::string& passwords)
{
	
}