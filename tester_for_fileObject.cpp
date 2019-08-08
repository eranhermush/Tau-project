
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
				 << std::endl << "3 - check the files(read files in a dir) " << std::endl << "4 - check the method 'go over the files' "
				  << std::endl << "5 - check string args "<< std::endl << "6 -work " << std::endl << "7 - exit " << std::endl;
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
			retVal = helpful_functions::file_to_file_object(a, path, to_print);
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
		    std::string result = "";
		    for (int i = 0; i< files_vec.size();i++)
		    {
		        if (i != 0)
		        {
		            result = result + "#";
		        }
		        result += files_vec[i];
		    }

		    index_vector_convertor index_vec_con;
		  	int user_input = 0;
		  	file_object f;
		  	f.intialize();
		    f.set_status(0);
		    //file_obj.set_scheme_msg(this->scheme_string);
		    f.set_scheme_msg(parser.get_str_original());
		    f.set_passwords(passwords);
		    f.set_password_function(password_functions);
		    f.set_files_for_scheme(result);
     		index_vec_con.intialize(f);



		  	std::cout << "your scheme is " << parser.get_str_original() << " and the compress is " << parser.get_str_compress() << std::endl;
		  	std::cout << "enter the indexes: " << parser.get_str_compress().length() << std::endl;

		    for (int i = 1; i <= parser.get_str_compress().length(); i++)
		    {
		  		cin >> user_input;
		        index_vec.push_back(user_input); 
		    }
			int index = index_vec_con.vector_indexes_to_index(index_vec);
			std::cout << "the index is: " << index << std::endl;
			std::vector<int> v = index_vec_con.index_to_vector_indexes(index);
			std::cout << "the vector is ";
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
		if (index_test == 6)
		{
			bool finish_loop = false;
			while (! finish_loop)
			{
				manager.go_over_files(to_print);
				finish_loop = manager.finish_job();
			}
		}			
		if (index_test == 7){
			finish = true;
		}
		if(index_test > 7 || index_test < 1){
			std::cout << "error :( your number is incorrect " << std::endl;
		}
	}
    return 0;

}
