#ifndef FILE_OBJECT_ARRAY_LIST_H
#define FILE_OBJECT_ARRAY_LIST_H
#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <stdlib.h> 
#include "file_object_node.h"

/**
 * This class is the array list of the file objects.
 */

class file_object_linked_list{

    file_object_node start;


public:
    file_object_linked_list(file_object_node& node);
    std::shared_ptr<file_object_node> search_node(file& f);
    void add_node(file_object_node& node);
    bool remove_node(file_object_node& node);

};


#endif //FILE_OBJECT_ARRAY_LIST_H