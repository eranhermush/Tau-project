
#include <iostream>

using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"

int main()
{
	parser_main a ("ccc");
	cout << a.from_known_char_to_options_list('c').at(0) << endl;


    return 0;

}