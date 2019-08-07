#include "client.h"

client::client(int id, std::string& dir_path, int sleep)
{
	this->id = id;
	this->dir_path = dir_path;
	this->microseconds_sleep = sleep;
}
bool client::start()
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

bool client::get_job(int status_start, bool to_write)
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

bool client::set_job(int status, std::string& passwords, int lines)
{
	std::string file_data;
	bool ret_val = false;
	std::string path =  this->dir_path + "/" + std::to_string(this->id) + ".txt";

	data = "";
	if(status == 6 && lines != 0)
	{
		data = std::to_string(status) + "\n" + this->file_obj.get_id() + "\n" + this->id + "\n" + std::to_string(lines) + "\n" + passwords;		
		ret_val =  helpful_functions::write_data_to_file(this->dir_path, std::to_string(this->id), "3"); // 3 indicates that we start a new worker
		if (! ret_val)
		{
			return false;
		}
		return true;
	}
	if(status != 6)
	{
		ret_val = helpful_functions::change_status_of_file(path, status);
		if (! ret_val)
		{
			return false;
		}
		return true;
	}
	return false;

}

bool client::work()
{
	bool retVal = false;
	std::string pass_str;
	std::vector<std::unique_ptr<Password_Generator>> generators;
	initialize_generators(generators);
	Nested_Password_Generator ngen(generators);
	if(this->file_obj.get_password_function() == "id")
	{
		Hash_Matcher<Id_Hash> hm1(Id_Hash(), this->file_obj.get_passwords());
		Preimage_Matcher match = hm1;
	}
	else
	{
		return false;
	}
	Preimage_Seeker seeker_for_passwords(ngen, match);
	std::vector<std::string> seek_all_results;
	seek_all_results = seeker_for_passwords.seek_all();
	helpful_functions::printcoll(seek_all_results);
	if(seek_all_results.size() == 0)
	{
		retVal =  set_job(2,"",0);
	}
	else
	{
		pass_str = vector_passwords_to_sring_passwords(seek_all_results);
		retVal =  set_job(6,pass_str,seek_all_results.size());
	}
	if (! retVal)
	{
		std::cout << "error in set job " << std::endl;
		return false;
	}
	return true;
}
void client::initialize_generators(std::vector<std::unique_ptr<Password_Generator>>& generators)
{
	parser.intialize(this->file_obj.get_scheme_msg());
	convertor.intialize(this->file_obj);
	std::vector<int> start_vector, finish_vector;
	start_vector = convertor.index_to_vector_indexes(this->file_obj.get_start_index());
	finish_vector = convertor.index_to_vector_indexes(this->file_obj.get_end_index());
	File_Password_Generator Pfile_gen("", 0, 0);
	Char_Pattern_Password_Generator Cfile_gen("", 0, 0);

	std::unique_ptr<Password_Generator> TempGen;
	generators.clear();
	for (int i = 0; i < this->parser.get_str_compress().length(); ++i)
	{
		if(this->parser.get_str_compress().at(i) == 'f')
		{
			Pfile_gen = File_Password_Generator( helpful_functions::index_of_file_object_to_fileindex(this->file_obj.get_scheme_msg(), i), start_vector.at(i), finish_vector.at(i));
			*TempGen = Pfile_gen;
		}
		else
		{
			Cfile_gen = Char_Pattern_Password_Generator( this->file_obj.get_scheme_msg().substr(i, this->parser.get_str_before_compress_size_at(i)), start_vector.at(i), finish_vector.at(i));
			*TempGen = Cfile_gen;			
		}
		generators.push_back(std::move(TempGen));
	}
}
std::string client::vector_passwords_to_sring_passwords(std::vector<std::string> &v)
{
	std::string result = "";
	for (int i = 0; i < v.size(); ++i)
	{
		if (i != 0)
		{
			result = result + "\n";
		}
		result = result + v.at(i);
	}
	return result;
}

void client::main()
{
	bool retVal = false;
	start();
	retVal = get_job(1, false);
	if (! retVal)
	{
		std::cout << "error in getting the first job" << std::endl;
		return;
	}
	retVal = work();
	if (! retVal)
	{
		std::cout << "error in working " << std::endl;
		return;
	}
}