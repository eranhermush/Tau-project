#ifndef HELPFUL_FUNCTIONS_H
#define HELPFUL_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

class helpful_functions
{
	public:
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

	    /*
	    * This function gets the string of the arguments from the server, and parses it and splits it to vectors of different type.
	    * The args (server string) string is as follows:
		* (We seperate the string to element, each element is in a new sentence.) - maybe we dont need
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

};
#endif