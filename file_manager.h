#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h> 
#include <algorithm>
#include "file_object.h"

/*
 * This class is a server class, it reads here the files and updates them
 */

class file_manager{

    file_object arr;
    file_object arr_didnt_do;
    std::vector< std::vector<std::string> > matrix_all_options;
    std::vector<std::string> file_names;
    std::string scheme_string;
    char file_char; // char of a file in the string scheme



public:
	/*
	 * constructor
	 * @param matrix_all_options- the matrix from the parser with all the options
     * @param file_names        - the names of our files (with repetitions) that we use in the scheme
	 */
    file_manager(std::vector< std::vector<std::string> > &matrix_all_options, std::vector<std::string> &file_names, std::string &scheme_string);
    /*
        This function validates that the input is valid:
            The size of the string == the size of the matrix
            There is a file name for each file option in the scheme
            it throws an error if the input is invalid
    */ 
    bool validate_input();

    //go_over_messages();
    //get_new_work();
    //bool generate_new_work_to_file();
    //bool check_file_need_to_update();
    std::vector<int>& index_to_vector_indexes(int index);
    int vector_indexes_to_index(std::vector<int> &vec);
    /*
    * This function gets an index in the scheme and returns the size of the object that this letter represents. e.g. the size of the file.
    */
    int size_of_object_in_scheme(int index);

    /*
        This function returns how many lines are in a file
    */
    int Get_number_of_lines_in_file(std::string filename);

};


#endif //FILE_MANAGER_H