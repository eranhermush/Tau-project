#include <iostream>

//using namespace std;
#include <bits/stdc++.h>
#include "parser_main.h"
#include <string>

#include "client.h"



int main()
{	


	int id;
	std::cout << "Hello, please enter theid:" << endl;
	cin >> id;
	client c(id, 'a');
	c.main();

    return 0;

}
