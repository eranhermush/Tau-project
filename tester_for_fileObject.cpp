
#include <iostream>

using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"
#include <string>
#include "worker_manager.h"
#include "file_manager.h"
#include "file_manager.h"

/*
	This function is for checking the matrix
*/
void print_matrix(vector< vector<string> > &matrix){
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

	cout << "Hello, please enter the target:" << endl;
	cin >> target;
	parser_main parser (parser_string);
	
	// create the matrix
	ret_value = parser.from_parser_string_to_matrix();
	if(ret_value == -1){
		cout << "Invalid input :( " << endl;
		return 0;
	}
	//print_matrix(parser.get_matrix());

	// creates the manager
	file_manager manager(parser.get_matrix());
	manager.main(target);


    return 0;

}