#include "file_object.h"

file_object::file_object()
{
	this->start_index = 0;
	this->end_index = 0;
	this->status = -1;
	this->id = -1;
	this->scheme_msg = "";
	this->passwords = "";
    this->password_function = "";
	this->files_for_scheme = "";
}


int file_object::get_id()
{
	return this->id;
}
void file_object::set_index(int start_index, int end_index)
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
        '\n' + std::to_string(this->start_index) + '\n' + std::to_string(this->end_index)+ '\n' + this->files_for_scheme + '\n' + this->passwords;
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
    std::string result = "id: " + std::to_string(this->id) + '\n' + "worker id: " + std::to_string(this->worker_id) + 
    '\n' + "scheme msg " + this->scheme_msg+ '\n' + "password functions : " + this->password_function + 
        '\n' + "start index " + std::to_string(this->start_index) + '\n' + + "end index " +std::to_string(this->end_index)+ 
        '\n' + "files" + this->files_for_scheme + '\n' + "passwords " + this->passwords;
    return result;
}
int file_object::get_start_index()
{
	return this->start_index;
}
int file_object::get_end_index()
{
	return this-> end_index;
}