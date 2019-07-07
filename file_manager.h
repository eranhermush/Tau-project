#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h> 
#include <algorithm>
#include <fstream>
#include "file_object.h"

/*
 * This class is a server class, it reads here the files and updates them
 */

class file_manager{

    file_object arr;
    file_object arr_didnt_do;
    std::vector< std::vector<std::string> > matrix_all_options;
    std::vector<std::string> file_names;
    std::string scheme_string, passwords, password_function;
    char file_char; // char of a file in the string scheme
    int curr_id;
    int sum_of_works;
    int current_index_of_work, work_size;




public:

    /*
    **********************************
    **********************************
    ******** Start Section ***********
    **********************************
    **********************************
    */

	/*
	 * constructor
	 * @param matrix_all_options- the matrix from the parser with all the options
     * @param file_names        - the names of our files (with repetitions) that we use in the scheme
	 */
    file_manager(std::vector< std::vector<std::string> > &matrix_all_options, std::vector<std::string> &file_names, std::string &scheme_string, std::string &passwords, std::string &password_function);
    /*
        This function validates that the input is valid:
            The size of the string == the size of the matrix
            There is a file name for each file option in the scheme
            it throws an error if the input is invalid
    */ 
    bool validate_input();
    /*
    * This function assumes that the class has a scheme with the options
    * it saves(to this->sum_of_works) how many options (passwords) we have in this model.
    */
    void save_sum_of_works();

    int get_sum_of_works();

    void set_work_size(int size);
    std::string get_files_in_string();
    //go_over_messages();
    //bool generate_new_work_to_file();
    //bool check_file_need_to_update();

    /*
    **********************************************************
    **********************************************************
    ***************   This is the index Section **************
    **********************************************************
    **********************************************************
    */


    /*
    * This function gets a general index, and splits it to a vector of indexes, index to every category in the scheme.
    */
    std::vector<int> index_to_vector_indexes(int index);
    /*
    * This function gets a vector of indexes, index to every category in the scheme and returns a general index that represents them.
    */
    int vector_indexes_to_index(std::vector<int> &vec);
    /*
    * This function gets an index in the scheme and returns the size of the object that this letter represents. e.g. the size of the file.
    */
    int size_of_object_in_scheme(int index);

    /*
    *    This function returns how many lines are in a file
    */
    int get_number_of_lines_in_file(std::string filename);


    /*
    **********************************************************
    **********************************************************
    ******************   Work section   **********************
    **********************************************************
    **********************************************************
    */

    /*
    * This function generates a new id for a file
    */
    int get_id_to_file();

    /*
    * This function creates a new work, and puts it in the file that it gets.
    * When the function returns, the file object is update and we can put it in the real file.
    * @ret: if the function returns -1 it means that we cant create the new file, since we go over all the possible passwords.
    */
    int create_new_work(file_object& file_obj);

};


#endif //FILE_MANAGER_H