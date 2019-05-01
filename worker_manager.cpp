

#include "worker_manager.h"

worker_manager::worker_manager(vector< vector<string> > matrix_all_options, string server_ip, int server_port)
{


	this->server_port = server_port;
	this->server_ip = server_ip;
    this->matrix_all_options = matrix_all_options;
    this->sum_of_works = 0;
    this->workers = {};
    this->main_socket = -1;
    for (int i = 0; i< matrix_all_options.size(); i++){
    	if(i == 0){
    		this->sum_of_works = matrix_all_options.at(i).size();
    	}
    	else{
    		this->sum_of_works *= matrix_all_options.at(i).size();
    	}
    }
    this->index_of_job_make_sure_thaat_made = 0;
    this->current_index_of_job_in_progress = 0;
    this->job_size = this->sum_of_works/2;
}
vector<vector<string>> worker_manager::intialize_worker()
{
	return this->matrix_all_options;
}

int worker_manager::get_job(int id) {
	this->current_index_of_job_in_progress += this->job_size;
	return this->job_size;
}

int worker_manager::get_job_size(){
	return this->job_size;
}

int worker_manager::create_server_of_worker_manager(){
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    //thread worker_handler;
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        return -1;
    } 
       

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons( this->server_port); 
  
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed"); 
        return -1;
    } 
    if (listen(server_fd, 10) < 0) 
    { 
        perror("listen"); 
        return -1;
    } 

    // wait for connections
    int i = 0;
    while(1){
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	    { 
	        perror("accept error"); 
	        return -1;
	    }
	    //worker_handler = thread(worker_handler_function(new_socket), i);
	    i++; 

	    // the id is 1
	    this->workers.push_back(worker_manager_worker_object(1, new_socket));
	    return 1; // Todo: remove it
    }

    

    return 0; 
} 

int worker_handler_function(int socket){
	int ret_val = 1;
	char buffer[1024] = {0}; 
	while (1){
		ret_val = read(socket, buffer, 1024);
		if (ret_val < 0){
			printf("error\n");
			return -1;
		}
	}
}