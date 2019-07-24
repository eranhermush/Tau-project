#ifndef HELPFUL_FUNCTIONS_H
#define HELPFUL_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
class helpful_functions
{
	public:
		/*
	    * This method goes over a directory and returns its files.
		* credit: 
		* http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
		*/
	    static void read_directory(std::string& name,  std::vector<std::string> v);
	    static void my_print(std::vector<std::string> &input);
 
};
#endif