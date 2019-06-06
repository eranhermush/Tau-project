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
    id: the message id (the id of this specific data) - real number
    status: the message status: 
        0: doesnt start to work on that
        1: in process
        2: done
    scheme_msg: the scheme string, like "ccCd"
    start_index and end index: represents the indexes of the password to go over them, in the original ordering
    files_for_scheme: if the scheme includes files, the file names will be here (or db identify), in order 
    password: The specific password that we want to go over, we assume that there is a file in the worker that has a function(encryption) with that name

 */

class file_object{

    int start_index, end_index, status, id;
    std::string scheme_msg, password, files_for_scheme;




public:
    file_object();
    int get_id();
    void set_index(int start_index, int end_index);
    void set_status(int status);
    void set_id(int id);
    void set_scheme_msg(std::string scheme_msg);
    void set_password(std::string password);
    void set_files(std::string files_for_scheme);
    bool operator==(const file_object& lhs);
    std::string get_file_message();


};


#endif //FILE_OBJECT_H