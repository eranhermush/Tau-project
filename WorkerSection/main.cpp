#include <iostream>
using namespace std;
#include <bits/stdc++.h>
#include <string>

#include "worker.h"


int main()
{	
	int server_port = 8810;
	int ret = 0;
	int index = 0;
	string result = "";
	cout << "Hello, Im client:" << endl;

	unique_ptr<worker> my_worker (new worker(server_port));
	ret = my_worker->intialize_worker_client();
	if(ret < 0){
		return 0;
	}
	ret = my_worker->get_target();
	if(ret < 0){
		return 0;
	}

	ret = my_worker->get_matrix();
	if(ret < 0){
		return 0;
	}

	ret = my_worker->get_work_size();
	if(ret < 0){
		return 0;
	}

	index =  my_worker->get_index();
	if(index < 0){
		return 0;
	}
	result = my_worker->work(index);

	ret = my_worker->send_message(result);
	if(ret < 0){
		return 0;
	}

	//worker* my_worker=  new worker();
	cout<< "result: " << result << endl;
	// my_worker.get_matrix(); Todo


    return 0;

}