#include <iostream>

//using namespace std;
#include <bits/stdc++.h>
#include <string>

#include "client.h"



int main()
{	


	int id;
	std::cout << "Hello, please enter the id:" << std::endl;
	std::cin >> id;

	std::string a = "a";
	client c(id, a,10000);
	c.main();
    return 0;

}
