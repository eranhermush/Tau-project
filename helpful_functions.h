#ifndef HELPFUL_FUNCTIONS_H
#define HELPFUL_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>


#include "file_object.h"

class helpful_functions
{
	public:


		/* Returns the size (in bytes) of the file in path
			If the file doesn't exist, or an error occured, or the file
			is empty, then 0 is returned (as there are no available bytes to read) */
		static uint64_t get_file_size(const std::string& path);

		/* Return the absolute path of the file in the relative path
			Returns an empty string on failure */
		static std::string get_absolute_path(const std::string& rel_path);


		/*
	    * This method goes over a directory and returns its files.
		* credit: 
		* http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
		*/
	    static void read_directory(std::string& name,  std::vector<std::string> &v);
	    /*
	    * prints a vector
	    */
	    static void my_print(std::vector<std::string> &input);
	    // credit: http://www.java2s.com/Tutorial/Cpp/0260__template/templatefunctiontoprintelementsofanSTLcontainer.htm
		template <typename T>
		static void printcoll (T const& coll)
		{
		    typename T::const_iterator pos;  // iterator to iterate over coll
		    typename T::const_iterator end(coll.end());  // end position

		    for (pos=coll.begin(); pos!=end; ++pos) {	
		        std::cout << *pos << ' ';
		    }
		    std::cout << std::endl;
		}

	    // split string 
	    // credit: https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
	    static size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
		/*
		* This function takse a scheme string and an index of file instance ('f') and returns the index of this instance in the files
		* for example, if the string is "ccfcfcfcf", and the index if 4 (str[4] = 'f') the function will return 1 (since this is the second 'f')
		* we assume that the input is valid
		*/
		static int index_of_file_object_to_fileindex(std::string& str, int index);
	    /*
	    * This function gets a file name, and transforms it to file_obj instance
	    * returns -1 if there was an error
	    * Warning: This function doesnt change the arrays of this class
	    * we assume that the file is valid -> was written by "write_work_to_file"
	    */
	    static int file_to_file_object(file_object& file_obj, std::string filename, bool print_error);


	    /*
	    * This function gets the string of the arguments from the server, and parses it and splits it to vectors of different type.
	    * The args (server string) string is as follows:
		* each element look like this:
		* X,d1,d2,args
		* Where X is the type: 0 = string, 1 = int, 2= float
		* d1 is the length (str length) of d2 (d1 is one digit)
		* d2 is the length (str length) of the args
		* args is the argument element to the hash function (like the seed)
		*
		* This function gets 3 stacks, it firsts cleans them, and then put all the elements of the string to these vectors, by there original order
		* The function returns True if the string was in this format, and otherwise returns false.
	    */
	    static bool server_string_to_vectors(std::string& server_str, std::vector<std::string> &str_vec, std::vector<int> &int_vec,std::vector<float> &float_vec);
	    /*
	    * credit: https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
	    */
	    static bool is_number( std::string& s);
	    /*
	    * credit: https://stackoverflow.com/questions/447206/c-isfloat-function
	    */
		static bool isFloat( std::string& myString );

	    /*
	    * This function gets the next integer from the string, returns false iff there was an error:
	    * start_index + size > len(str) 
	    * size < 1
	    * str[start_index: start_index+size] is not a number
		*
	    * it puts the number in the result varirable
	    */
	    static bool get_next_int(std::string &str, int start_index, int size, int& result);

	    static bool write_data_to_file(std::string& dir, std::string filename, const std::string& data_to_file);
	    /*
	    * This function changes the first line of the file to the desired status
	    * if the status is not a one digit (between 0 to 9) we return false and not put it in the file
	    * we also return false if there was a problem in one of the file operations
	    */
	    static bool change_status_of_file(std::string& path, int status);
};
#endif