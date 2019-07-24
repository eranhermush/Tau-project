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
#include "file_object.h"
#include "helpful_functions.h"
/*
 * This class is a server class, it reads here the files and updates them
 */

class file_manager{

    std::vector<file_object> arr_of_works;
    std::vector<file_object> arr_didnt_do;
    std::vector< std::vector<std::string> > matrix_all_options;
    std::vector<std::string> file_names;
    std::string scheme_string, passwords, password_function;
    char file_char; // char of a file in the string scheme
    int curr_id;
    int sum_of_works;
    int current_index_of_work, work_size;
    std::string dir_path;




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
    file_manager(std::string path, std::vector< std::vector<std::string> > &matrix_all_options, std::vector<std::string> &file_names, std::string &scheme_string, std::string &passwords, std::string &password_function);
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
    * This function creates a new work, and puts it in the file object that it gets.
    * it also adds the work to the arr of the class
    * When the function returns, the file object is update and we can put it in the real file.
    * @ret: if the function returns -1 it means that we cant create the new file, since we go over all the possible passwords.
    */
    int create_new_work(file_object& file_obj, int worker_id);
    /*
    * This function gets a file_object instance, and writes it to a file
    * It first writes all the data, and then write the status (it does for consistency)
    */
    int write_work_to_file(file_object& file_obj);

    /*
    * This function gets a file name, and transforms it to file_obj instance
    * returns -1 if there was an error
    * Warning: This function doesnt change the arrays of this class
    * we assume that the file is valid -> was written by "write_work_to_file"
    */
    int file_to_file_object(file_object& file_obj, std::string filename);

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
    void go_over_files();


};


#endif //FILE_MANAGER_H