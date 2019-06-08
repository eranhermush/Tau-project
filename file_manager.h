#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <stdlib.h> 
#include "file_object.h"

/**
 * This class is a server class, it reads here the files and updates them
 */

class file_manager{

    file_object arr;
    file_object arr_didnt_do;



public:
    file_manager();
    go_over_messages();
    get_new_work()
    generate_new_work_to_file();
    check_file_need_to_update();





};


#endif //FILE_MANAGER_H