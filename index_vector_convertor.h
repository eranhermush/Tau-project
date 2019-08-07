#ifndef INDEX_TO_VECTOR_H
#define INDEX_TO_VECTOR_H
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <stdlib.h> 
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include "parser_main.h"
#include "file_object.h"
#include "helpful_functions.h"


class index_vector_convertor{

    parser_main parser;
    file_object fileobj;
    char file_char;

public:

    index_vector_convertor(char file_char = 'f');

    void intialize(file_object& fileobj);
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

};


#endif //FILE_OBJECT_H