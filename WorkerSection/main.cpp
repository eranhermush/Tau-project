#include <iostream>
using namespace std;
#include <bits/stdc++.h>
#include <string>

#include "worker.h"


int main()
{	
	int server_port = 8811;
	cout << "Hello, client:" << endl;
	unique_ptr<worker> my_worker (new worker());
	//worker* my_worker=  new worker();
	// my_worker.get_matrix(); Todo
	cout <<"Im here" <<endl;


    return 0;

}