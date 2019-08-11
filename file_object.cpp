#include "file_object.h"

file_object::file_object()
{
	intialize();
}


int file_object::get_id()
{
	return this->id;
}
void file_object::set_index(uint64_t  start_index, uint64_t  end_index)
{
	this->start_index = start_index;
	this->end_index = end_index;
}
void file_object::set_status(int status)
{
	this->status = status;
}
void file_object::set_id(int id)
{
	this->id = id;
}
void file_object::set_scheme_msg(std::string scheme_msg)
{
	this->scheme_msg = scheme_msg;
}
void file_object::set_passwords(std::string passwords)
{
	this->passwords = passwords;
}
void file_object::set_password_function(std::string password_function)
{
    this->password_function = password_function;
}
void file_object::set_files_for_scheme(std::string files_for_scheme)
{
	this->files_for_scheme = files_for_scheme;
}
int file_object::get_status()
{
    return this->status;
}
std::string file_object::get_message_to_write_in_file_without_status()
{	
    std::string result = std::to_string(this->id) + '\n' + std::to_string(this->worker_id) + '\n' + this->scheme_msg+ '\n' + this->password_function + 
        '\n' + std::to_string(this->start_index) + '\n' + std::to_string(this->end_index)+ '\n' + this->files_for_scheme + '\n' + this->passwords  + '\n' + this->arguments_to_hash;
    if(this->files_for_scheme.length() == 0)
    {
		result = std::to_string(this->id) + '\n' + std::to_string(this->worker_id) + '\n' + this->scheme_msg+ '\n' + this->password_function + 
        	'\n' + std::to_string(this->start_index) + '\n' + std::to_string(this->end_index)+  '\n' + this->passwords + '\n' + this->arguments_to_hash;
    }
    return result;
}
std::string file_object::get_message_to_write_in_file()
{
	std::string result = std::to_string(this->status) + '\n' + get_message_to_write_in_file_without_status();
	return result;
}
void file_object::set_worker_id(int id)
{
	this->worker_id = id;
}
int file_object::get_worker_id()
{
	return this->worker_id;
}
std::string file_object::to_string()
{
    std::string result = "status: " + std::to_string(this->status) + '\n' +"id: " + std::to_string(this->id) + '\n' + "worker id: " + std::to_string(this->worker_id) + 
    	'\n' + "scheme msg " + this->scheme_msg+ '\n' + "password functions : " + this->password_function + 
        '\n' + "start index " + std::to_string(this->start_index) + '\n' + "end index " + std::to_string(this->end_index)+ 
        '\n' + "files" + this->files_for_scheme + '\n' + "passwords " + this->passwords + '\n' + "arguments to hash:  " + this->arguments_to_hash;
    return result;
}
uint64_t file_object::get_start_index()
{
	return this->start_index;
}
uint64_t file_object::get_end_index()
{
	return this-> end_index;
}
std::string file_object::get_scheme_msg()
{
	return this->scheme_msg;
}
std::string file_object::get_passwords()
{
	return this->passwords;
}
std::string file_object::get_password_function()
{
	return this->password_function;
}
std::string file_object::get_files_for_scheme()
{
	return this->files_for_scheme;
}
bool file_object::check_equal(file_object& obj)
{

	if (this->worker_id != obj.get_worker_id()){
		return false;
	}
	if (this->start_index != obj.get_start_index()){
		return false;
	}
	if (this->end_index != obj.get_end_index()){
		return false;
	}
	if (this->id != obj.get_id()){
		return false;
	}

    if  (!(this->scheme_msg == obj.get_scheme_msg() )) 
    {
        return false;
    }  

    if  (!(this->passwords  == obj.get_passwords() ))
    {
        return false;
    }   

    if  (!(this->password_function == obj.get_password_function() ))
    {
        return false;
    }

    if  (!(this->files_for_scheme == obj.get_files_for_scheme() ))
    {
    	//std::cout << "here" << this->files_for_scheme << " ttt " <<this->files_for_scheme.length()<< " ttt " << obj.get_files_for_scheme() << " ttt " << obj.get_files_for_scheme().length() << std::endl;
        return false;
    } 
    if  (!(this->arguments_to_hash == obj.get_arguments() ))
    {
        return false;
    } 
    return true;
}
void file_object::intialize()
{
	this->passwords_found_vector.clear();
	this->start_index = -1;
	this->end_index = -1;
	this->status = -1;
	this->id = -1;
	this->worker_id = -1;
	this->scheme_msg = "";
	this->password_function = "";
	this->passwords = "";
	this->files_for_scheme = "";
	this->arguments_to_hash = "";
}
void file_object::intialize_to_finish_work()
{
	this->passwords_found_vector.clear();
	this->status = 5;
	this->scheme_msg = "";
	this->password_function = "";
	this->passwords = "";
	this->files_for_scheme = "";
	this->arguments_to_hash = "";
}
void file_object::set_arguments(std::string &args)
{
	this->arguments_to_hash = args;
}
std::string file_object::get_arguments()
{
	return this->arguments_to_hash;
}
void file_object::add_password_found_vector(std::string &pass)
{
	this->passwords_found_vector.push_back(pass);
	return;
}
std::vector<std::string> file_object::get_passwords_found_vector()
{
	return this->passwords_found_vector;
}