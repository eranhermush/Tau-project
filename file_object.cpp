#include "file_object.h"

file_object::file_object(){
	this->start_index = 0;
	this->end_index = 0;
	this->status = -1;
	this->id = -1;
	this->scheme_msg = "";
	this->password = "";
	this->files_for_scheme = "";
}

void file_object



    void file_object::set_index(int start_index, int end_index){
    	this->start_index = start_index;
    	this->end_index = end_index;
    }
    void file_object::set_status(int status);
    void file_object::set_id(int id);
    void file_object::set_scheme_msg(std::scheme_msg);
    void file_object::set_password(std::password);
    void file_object::set_files(std::files_for_scheme);
    std::string file_object::get_file_message();