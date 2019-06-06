#include "file_object_node.h"

file_object_node::file_object_node(file_object& file)
	this->file_object_data = file;
	this->before = nullptr;
	 this->after = nullptr;
}


