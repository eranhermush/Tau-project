#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H
#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <stdlib.h> 


#include <sys/types.h> 


/**
 * This is a class that represents a file in the protocol between the workers and the server.

 * The file includes:
    id:                         The message id (the id of this specific data) - real number
    status:                     The message status: 
                                    0: doesnt start to work on that
                                    1: in process
                                    2: done
                                    3: Start worker
                                    4: Finish Work (and done this work)
                                    5: There are no messages any more (a message from server to client instead of a new work)               
    scheme_msg:                 The scheme string, like "ccCd"
    start_index and end index:  Represents the indexes of the password to go over them, in the original ordering
    files_for_scheme:           If the scheme includes files, the file names will be here (or db identify), in order 
    password_function:          The specific password fuction that we want to go over, we assume that there is a file in the worker that has a function(encryption) with that name
    passwords:                  The passwords that we want to find
    worker_id:                  The worker id

 */

class file_object{

    int start_index, end_index, status, id, worker_id;
    std::string scheme_msg, passwords, files_for_scheme, password_function;



public:
    file_object();
    void set_index(int start_index, int end_index);
    void set_status(int status);
    void set_id(int id);
    void set_scheme_msg(std::string scheme_msg);
    void set_passwords(std::string passwords);
    void set_password_function(std::string password_function);
    void set_files_for_scheme(std::string files_for_scheme);
    void set_worker_id(int id);

    int get_worker_id();
    int get_start_index();
    int get_end_index();
    int get_id();
    std::string get_scheme_msg();
    std::string get_passwords();
    std::string get_password_function();
    std::string get_files_for_scheme();

    /*
    * This function returns the message that we will write in the file
    */
    std::string get_message_to_write_in_file();

    int get_status();
    std::string get_message_to_write_in_file_without_status();
    std::string to_string();

    // we checks here if the files are equal
    bool check_equal(file_object& obj);

    // intialize the object to default values 
    void intialize();

    // intialize the object to error values  (-1/"" to all, expect status, which is 5)   
    void intialize_to_error();

};


#endif //FILE_OBJECT_H