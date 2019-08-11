#ifndef TAU_PROJECT_CLIENT_H
#define TAU_PROJECT_CLIENT_H
#include "password_generator.h"
#include "../file_object.h"
#include "../helpful_functions.h"
#include "preimage_seeker.h"
#include "nested_password_generator.h"
#include "char_password_generator.h"
#include "file_password_generator.h"
#include "hash_matcher.h"
#include "generator_creator.h"
#include "Id_Hash.h"
#include "preimage_matcher_by_name.h"
#include <string>
#include <vector>


class client
{
	file_object file_obj; // this file will save our current job;

	int id, microseconds_sleep;
	std::string dir_path;
public:

	client(int id, std::string& dir_path, int sleep = 1000);
	/*
	* This function gets a job - it creates the client file and waits for the first job
	* This function returns false iff there was an error with the file operations
	*/
	bool start();
	/*
	* This function gets a job - it creates a request to the sever for a job, and waits to its response, and when it answers it returns the job
	* status_start is the status field in the file (first line)
	* to_write indicates if we need to write to the file the status_start or not
	* 		(and then just to wait for the server to give us a job. it can happend when the client writes it in a different funciton)
	* This function returns false iff there was an error with the file operations
	*/
	bool get_job(int status_start, bool to_write);
	/*
	* passwords is the passwords that we found. if there are no passords, lines should be 0, 
	* 		(Warning): and if there are passwords, status must be 6, otherwise we will not write it to the file and return false!!!
	* lines = how many line (= how many passwords) are in passwords
	* all the other fields are like the former functions
	*/
	bool set_job(int status, const std::string& passwords, int lines);

	/*
	* This function should be called when there is a job i.e. when the file_obj is initialized with a new job
	* this function makes the real work of the client - goes over all the passwords and checks them
	* now, we support only the id function as a hash function :), it doesnt need any arguments (that are not the password)
	* returns false if there was an error in the work
	*/
	bool work();
	/*
	* this function is called when we have a job, it intializes all the password generators in accordance with the file
	*/
	std::string vector_passwords_to_sring_passwords(std::vector<std::string> &v);
	void main();
};


#endif