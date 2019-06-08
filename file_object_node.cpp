#include "file_object_node.h"

file_object_node::file_object_node(file_object& file)
	this->file_object_data = file;
	this->before = nullptr;
	 this->after = nullptr;
}


std::shared_ptr<file_object_node> file_object_node::get_before(){
	return this->before;
}

void file_object_node::set_before(std::shared_ptr<file_object_node> file){
	this->before = file;
}


std::shared_ptr<file_object_node> file_object_node::get_after(){
	return this->after;
}

void file_object_node::set_after(std::shared_ptr<file_object_node> file){
	this->after = file;
}

file_object& file_object_node::get_file(){
	return this->file_object_data;
}

void file_object_node::set_file(file_object& file){
	this->file_object_data = file;
}