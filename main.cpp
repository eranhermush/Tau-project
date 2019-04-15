
#include <iostream>

using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"
#include <string>
#include "worker_manager.h"

int main()
{	
	string parser_string;
	cout << "Hello, please enter the parser string:" << endl;
	cin >> parser_string;
	parser_main a (parser_string);
	
	cout << a.from_known_char_to_options_list('c').at(0) << endl;


    return 0;

}