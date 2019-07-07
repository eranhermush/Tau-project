
#include <iostream>

//using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"
#include <string>
#include "worker_manager.h"
#include "file_manager.h"

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
	int server_port = 8810;
	string parser_string;
	string target;
	int ret_value = 0;
	cout << "Hello, please enter the parser string:" << endl;
	cin >> parser_string;

    std::vector<int> index_vec; 
  	int user_input = 0;
  	cout << "enter the indexes: " << parser_string.length() << endl;

    for (int i = 1; i <= parser_string.length(); i++)
    {
  		cin >> user_input;
        index_vec.push_back(user_input); 
    }
    std::vector<std::string> files_vec; 
  	std::string user_input_file = "";
  	cout << "enter the files" << std::count(parser_string.begin(), parser_string.end(),'f') << endl;
    for (int i = 1; i <= std::count(parser_string.begin(), parser_string.end(),'f'); i++)
    {
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
	//print_matrix(parser.get_matrix());

	// creates the manager
	file_manager manager(parser.get_matrix(),files_vec,parser_string);
	int index = manager.vector_indexes_to_index(index_vec);
	cout << "the index is: " << index << endl;
	std::vector<int> v = manager.index_to_vector_indexes(index);
	cout << "the vector is ";
	for (int i = 0; i < v.size(); i++) {
		std::cout << v.at(i) << ' ';
	}
	std::cout << std::endl;
	//matrix_all_options, std::vector<std::string> &file_names, std::string &scheme_string)


    return 0;

}