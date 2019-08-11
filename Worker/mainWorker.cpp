
#include <iostream>

//using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"
#include <string>

#include "worker_manager.h"
#include "file_manager.h"
#include "helpful_functions.h"

/*
	This function is for checking the matrix
*/
void print_matrix(std::vector< std::vector<std::string> > &matrix){
	for(int i=0;i<matrix.size();i++){
		for(int j=0; j<matrix.at(i).size(); j++){
			cout << matrix.at(i).at(j);
		}
		cout <<endl;
	}
}

int main()
{	




	string parser_string;
	string target;
	int ret_value = 0;
	int Sum_of_workers = 0;
	bool to_print = true;
	std::string path, dir_path = "a";

	std::vector<std::string> str_vec;
	std::vector<int> int_vec;
	std::vector<float> float_vec;

	std::cout << "Hello, please enter the parser string:" << endl;
	cin >> parser_string;


    std::vector<std::string> files_vec; 
  	std::string user_input_file = "";
    for (int i = 1; i <= std::count(parser_string.begin(), parser_string.end(),'f'); i++)
    {
    	std::cout << "enter the file number " << i << " (we have " << std::count(parser_string.begin(), parser_string.end(),'f') << " files)" << std::endl;
  		cin >> user_input_file;
        files_vec.push_back(user_input_file); 
    }
	parser_main parser (parser_string);
	
	// create the matrix
	ret_value = parser.from_parser_string_to_matrix();
	if(ret_value == -1){
		cout << "Invalid input :( " << endl;
		return 0;
	}
	std::string password_functions, hash_args;
	std::string passwords;

	cout << "enter the passwords"  << endl;
	cin >> passwords;
	cout << "enter the password functions"  << endl;
	cin >> password_functions;
	cout << "enter the arguments"  << endl;
	cin >> hash_args;

	// creates the manager
	file_manager manager("a",parser.get_matrix(),files_vec,parser_string,passwords, password_functions, hash_args);

	int work_id = 0;
	int retVal = 0;
	bool str_vec_bool;

	std::cout << "enter sum of workers " << std::endl;
	cin >> Sum_of_workers;
	

	
    return 0;

}
