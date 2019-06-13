#include "file_object.h"

file_object::file_object(){
	this->start_index = 0;
	this->end_index = 0;
	this->status = -1;
	this->id = -1;
	this->scheme_msg = "";
	this->password = "";
	this->files_for_scheme = "";
    this->objects_in_scheme =0;
}


	int file_object::get_id(){
		return this->id;
	}
    void file_object::set_index(int start_index, int end_index){
    	this->start_index = start_index;
    	this->end_index = end_index;
    }
    void file_object::set_status(int status){
    	this->status = status;
    }
    void file_object::set_id(int id){
    	this->id = id;
    }
    void file_object::set_scheme_msg(std::string scheme_msg){
    	this->scheme_msg = scheme_msg;
        this->objects_in_scheme = scheme_msg.length();
    }
    int file_object::get_objects_in_scheme(){
        return this->objects_in_scheme;
    }
    void file_object::set_password(std::string password){
    	this->password = password;
    }
    void file_object::set_files(std::string files_for_scheme){
    	this->files_for_scheme = files_for_scheme;
    }
    std::string file_object::get_file_message(){
    	std::string result = this->status + '\n' + std::to_string(this->id) + '\n' + std::to_string(this->status)+ '\n' + this->scheme_msg + 
    		'\n' + std::to_string(this->start_index) + '\n' + std::to_string(this->end_index)+ '\n' + this->files_for_scheme + '\n' + this->password;
    	return result;
    }
    bool operator==(const file_object& lhs){
    	return lhs.get_id() == this->id;
    }
