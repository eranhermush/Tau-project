#include <iostream>

//using namespace std;
#include <bits/stdc++.h>
#include <string>

#include "client.h"



int main()
{	


	int id;
	std::cout << "Hello, please enter theid:" << std::endl;
	std::cin >> id;
	std::string a = "a";
	client c(id, a);
	c.main();

    return 0;

}
