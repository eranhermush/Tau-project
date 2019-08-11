#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h> 
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <exception>
#include "file_object.h"
#include "helpful_functions.h"
#include "pattern_utils.h"

/*
 * This class is a server class, it reads here the files and updates them
 */

class file_manager{

    std::vector<file_object> arr_of_works;
    std::vector<file_object> arr_didnt_do;
    //std::vector< std::vector<std::string> > matrix_all_options;
    std::vector<std::string> file_names;
    std::string scheme_string, passwords, password_function, hash_args;
    char file_char; // char of a file in the string scheme
    int curr_id;
    uint64_t sum_of_works, current_index_of_work, start_index_work;
    unsigned int  work_size;
    std::string dir_path;
    file_object fileobj;
    int last_printed;

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
    file_manager(const std::string& path, std::string& scheme_string, std::vector<std::string> &file_names, 
        std::string &passwords, std::string &password_function, std::string &hash_args, uint64_t start, uint64_t finish, unsigned int size_of_job);
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

    uint64_t get_sum_of_works();

    void set_work_size(unsigned int size);
    std::string get_files_in_string();
    //go_over_messages();
    //bool generate_new_work_to_file();
    //bool check_file_need_to_update();

    /*
    * This function checks if we have more jobs and more things to do
    */
    bool finish_job();

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

    void update_file_object_no_index(file_object& f);

    /*
    * This function creates a new work, and puts it in the file object that it gets.
    * it also adds the work to the arr of the class
    * When the function returns, the file object is update and we can put it in the real file.
    * @ret: if the function returns -1 it means that we cant create the new file, since we go over all the possible passwords.
    */
    int create_new_work(file_object& file_obj, int worker_id);
    /*
    * This function gets a file_object instance, and writes it to a file
    * It first writes all the data, and then write the status (it does for consistency)
    *  ****** This file changes the array of the working files ***********
    */
    int write_work_to_file(file_object& file_obj);

    /*
    * This function goes over all the files in the directory. For each file it does:
    * 1. Checks that the file is a valid file - i.e. it is in our protocol of writing to file.
    * 2. Checks that the workerid in the file == the filename
    * 3. Checks that the data in the file is consistent with the data in the array
    * 4. Checks that it is not a new worker (status != 3)
    * If the answer to one (or more) of questions(1-3) and question 4 were false, this functions does the following:
    *    a. removes the worker from the worker arr
    *    b. add this work to the didntWork arr
    *    c. delete the file
    * If the answers were true we does:
    *    a. Checks the status. if the status is 1 or 0 - we ignore the file (maybe later we will add timing element)
    *    b. If the status is 2 - we removes this work from the arr, and update the file with a new work
    *    c. If the status is 3 we gives the worker a new work.
    */
    bool go_over_files( bool print_error);

    /*
    * This function gets a file name and returns if this file is validate - like the protocol.
    * print_error indicates if we want to print to stdout the error
    */
    bool check_validate_of_file(std::string file_name, std::string full_file_name, file_object& file_obj, bool print_error);
};


#endif //FILE_MANAGER_H