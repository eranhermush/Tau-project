#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include <unistd.h>
#include <stdlib.h> 


#include <sys/types.h> 


/**
 * This is a class that represents a file in the protocol between the workers and the server.

 * The file includes:
    id:                         The message id (the id of this specific data) - real number
    status:                     The message status: 
                                    0: doesn't start to work on that
                                    1: in process
                                    2: done
                                    3: Start worker
                                    4: Finish Work (and done this work)
                                    5: There are no messages any more (a message from server to client instead of a new work) 
                                    6: a worker message - I found passwords!!!!!
                                             at this mode the file will be different: status, id, worker_id, how many passwords I found (int), all the passwords(a password in a line) :)  
                                    7: I write now to the file, ignore it 
                                     
    scheme_msg:                 The scheme string, like "ccCd"
    start_index and end index:  Represents the indexes of the password to go over them, in the original ordering
    files_for_scheme:           If the scheme includes files, the file names will be here (or db identify), in order 
    password_function:          The specific password function that we want to go over, we assume that there is a file in the worker that has a function(encryption) with that name
    passwords:                  The passwords that we want to find
    worker_id:                  The worker id
    arguments:                  Arguments to the hash functions, see server_string_to_vectors in helpful_functions.h to more details

 */

class file_object{

    uint64_t  start_index, end_index;;
    int status, id, worker_id;
    std::string scheme_msg, passwords, files_for_scheme, password_function, arguments_to_hash;
    std::vector<std::string> passwords_found_vector;



public:

    static const int dont_start_working_symbol = 0;
    static const int working_in_process_symbol = 1;
    static const int done_working_symbol = 2;
    static const int start_worker_symbol = 3;
    static const int finish_work_symbol = 4;
    static const int no_more_works_symbol =5;
    static const int worker_found_password_symbol = 6;
    static const int worker_write_to_file_symbol = 7;
    static const char delimiter_of_files_in_fileobject_symbol = '\n';//char(1);

    file_object();
    void set_index(uint64_t  start_index, uint64_t  end_index);
    void set_status(int status);
    void set_id(int id);
    void set_scheme_msg(std::string scheme_msg);
    void set_passwords(std::string passwords);
    void set_password_function(std::string password_function);
    void set_files_for_scheme(std::string files_for_scheme);
    void set_worker_id(int id);
    void set_arguments(std::string &args);
    void add_password_found_vector(std::string &pass);

    std::vector<std::string> get_passwords_found_vector();
    int get_worker_id();
    uint64_t get_start_index();
    uint64_t get_end_index();
    int get_id();
    std::string get_scheme_msg();
    std::string get_passwords();
    std::string get_password_function();
    std::string get_files_for_scheme();
    std::string get_arguments();
    /*
    * This function returns the message that we will write in the file
    */
    std::string get_message_to_write_in_file();

    int get_status();
    std::string get_message_to_write_in_file_without_status();
    std::string to_string();

    // we checks here if the files are equal
    // Warning: this function doesn't compare the status tag
    bool check_equal(file_object& obj);

    // initialize the object to default values 
    void intialize();

    // initialize the object to error values  (-1/"" to all, expect status, which is 5)   
    void intialize_to_finish_work();

};


#endif //FILE_OBJECT_H