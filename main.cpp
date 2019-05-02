
#include <iostream>

using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"
#include <string>
#include "worker_manager.h"


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
	string parser_string;
	cout << "Hello, please enter the parser string:" << endl;
	cin >> parser_string;
	parser_main parser (parser_string);
	
	// create the matrix
	parser.from_parser_string_to_matrix();
	print_matrix(parser.get_matrix());


    return 0;

}