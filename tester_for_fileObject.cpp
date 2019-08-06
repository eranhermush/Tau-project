
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
	int index_test = 0;
	bool to_print = true;
	bool finish = false;
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
	//print_matrix(parser.get_matrix());

	// creates the manager
	file_manager manager(dir_path, parser, files_vec, passwords, password_functions, hash_args);

	int work_id = 0;
	int retVal = 0;
	bool str_vec_bool;

	while (! finish)
	{
		std::cout << std::endl << "enter an index (choose what to do): " << std::endl << "1 - add a work to file " << std::endl << "2 - check the index section "
				 << std::endl << "3 - check the files(read files in a dir) " << std::endl << "4 - check the method 'go over the files' " << std::endl << "5 - check string args "<< std::endl << "6 - exit " << std::endl;
		cin >> index_test;


		if(index_test == 1)
		{
			file_object a;
			std::cout << "enter the worker id " << std::endl;
			cin >> work_id;
			path =  dir_path + "/" + std::to_string(work_id) + ".txt";

			int result = manager.create_new_work(a, work_id);
			//std::cout << "first result is " << result << std::endl;
			retVal =  manager.write_work_to_file(a);
			if (retVal == -1){
				std::cout << "error in write_work_to_file " << std::endl;
			}
			//std::cout << "finish" << std::endl;
			retVal = manager.file_to_file_object(a, path, to_print);
			if (retVal == -1){
				std::cout << "error in file_to_file_object " << std::endl;
			}
			else{
				std::cout << "done " << std::endl;

			}
			//std::cout << a.to_string() << std::endl;			
		}
		if(index_test == 2)
		{
			/*
			This is the tester for the indexes section
			*/
		    std::vector<int> index_vec; 
		  	int user_input = 0;
		  	cout << "enter the indexes: " << parser_string.length() << endl;

		    for (int i = 1; i <= parser_string.length(); i++)
		    {
		  		cin >> user_input;
		        index_vec.push_back(user_input); 
		    }
			int index = manager.vector_indexes_to_index(index_vec);
			cout << "the index is: " << index << endl;
			std::vector<int> v = manager.index_to_vector_indexes(index);
			cout << "the vector is ";
			for (int i = 0; i < v.size(); i++) {
				std::cout << v.at(i) << ' ';
			}
			std::cout << std::endl;
		}

		if (index_test == 3)
		{
			std::vector<std::string> file_names;
			std::string directory_name;
			cout << "enter the directory name "  << endl;
			cin >> directory_name;
			helpful_functions::read_directory(directory_name, file_names);
			helpful_functions::my_print(file_names);
		}
		if (index_test == 4)
		{
			manager.go_over_files(to_print);
		}
		if (index_test == 5)
		{

			str_vec_bool =  helpful_functions::server_string_to_vectors(hash_args, str_vec, int_vec, float_vec);
			if (! str_vec_bool)
			{
				std::cout << "There was an error in server_string_to_vectors" << std::endl;
			}
			else
			{
				std::cout << "str vec: " << std::endl;
				helpful_functions::printcoll(str_vec);
				std::cout << "int vec: " << std::endl;
				helpful_functions::printcoll(int_vec);
				std::cout << "float vec: " << std::endl;
				helpful_functions::printcoll(float_vec);

			}
		}				
		if (index_test == 6){
			finish = true;
		}
		if(index_test > 6 || index_test < 1){
			std::cout << "error :( your number is incorrect " << std::endl;
		}
	}
    return 0;

}
