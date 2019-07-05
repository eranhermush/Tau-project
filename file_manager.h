#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <stdlib.h> 
#include "file_object.h"

/*
 * This class is a server class, it reads here the files and updates them
 */

class file_manager{

    file_object arr;
    file_object arr_didnt_do;
    std::vector< std::vector<std::string> > matrix_all_options;



public:
	/**
	 * constructor
	 * @param str- the matrix from the parser with all the options
	 */
    file_manager(std::vector< std::vector<std::string> > &matrix_all_options);
    //go_over_messages();
    //get_new_work();
    //bool generate_new_work_to_file();
    //bool check_file_need_to_update();
    std::vector<int>& index_to_vector_indexes(int vector);
    int vector_indexes_to_index(std::vector<int> &vec);
    /*
    * This function gets an index in the scheme and returns the size of the object that this letter represents. e.g. the size of the file.
    */
    int scheme_at_index_to_len(int index);
};


#endif //FILE_MANAGER_H