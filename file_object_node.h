#ifndef FILE_OBJECT_ARRAY_LIST_NODE_H
#define FILE_OBJECT_ARRAY_LIST_NODE_H
#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <stdlib.h> 
#include "file_object.h"

/**
 * This class is a node of file object, for the array list.
 */

class file_object_node{

    file_object file_object_data;
    std::shared_ptr<file_object_node> before, after;



public:
    file_object_node(file_object& file);
    std::shared_ptr<file_object_node> get_before();
    void set_before(std::shared_ptr<file_object_node> file);
    std::shared_ptr<file_object_node> get_after();
    void set_after(std::shared_ptr<file_object_node> file);
    file_object& get_file();
    void set_file(file_object& file);

};


#endif //FILE_OBJECT_ARRAY_LIST_NODE_H