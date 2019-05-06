

#include "worker_manager.h"

worker_manager::worker_manager(vector< vector<string> > &matrix_all_options, int server_port, string user_input)
{


	this->server_port = server_port;
    this->user_input = user_input.c_str();
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
    address.sin_port = htons(this->server_port); 
  
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
        cout << "waiting for clients" <<endl;
	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	    { 
	        perror("accept error"); 
	        return -1;
	    }
        cout << "get client" << endl;
	    //worker_handler = thread(worker_handler_function(new_socket), i);
	    i++; 

	    // the id is 1
	    this->workers.push_back(worker_manager_worker_object(1, new_socket));
	    return 1; // Todo: remove it
    }

    

    return 0; 
} 

int worker_manager::worker_handler_function(int socket){
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

int worker_manager::send_message(int client_index, string message){
    int ret = 0;
    ret = send(this->workers.at(client_index).get_socket() , message.c_str() , message.length() , 0 ); 
    //ret = send(this->workers.at(client_index).get_socket() , this->user_input , strlen(this->user_input) , 0 ); 
    if (ret <0){
        return -1;
    } 
    return 0;
}

int worker_manager::send_work_size_and_index(unsigned int work_size, unsigned int index, int client_index)
{
    int ret = 0;
    unsigned int* val;
    ret = another_functions::send_int(work_size, this->workers.at(client_index).get_socket());
    
    if (ret <0 ){
        perror("error send work_size:");
        return -1;
    }
    ret = another_functions::send_int(index, this->workers.at(client_index).get_socket());
    
    if (ret <0 ){
        perror("error send index:");
        return -1;
    }
    return 0;
}

string worker_manager::get_message(int client_index)
{
    char message[1024] = {0};
    int valread = 0;
    valread = read(this->workers.at(client_index).get_socket() , message, 1024); 
    if (valread <0 ){
        perror("error get_message:");
        return "";
    }
    return message;
}
int worker_manager::main(string target){
    create_server_of_worker_manager();
    string str = "";
    int ret = send_message(0,target);
    if(ret <0){
        perror("error target: ");
        return 0;
    }

    ret = send_message(0,this->user_input);
    if(ret <0){
        perror("error user_input: ");
        return 0;
    }
    ret = this->send_work_size_and_index(this->sum_of_works,0,0);
    if(ret <0){
        perror("error send_work_size_and_index: ");
        return 0;
    }
    str = this->get_message(0);
    cout<<"found: " << str << endl;
    //ret = my_worker->get_target();
    //ret = my_worker->get_matrix();
    //ret = my_worker->get_work_size();
    //index =  my_worker->get_index();
    //ret = my_worker->send_message(result);



    return 1;
}
int worker_manager::send_matrix_to_worker(int index){
    int ret = 0;
    //worker my_worker;
    //my_worker = this->workers.at(i);
    for(int i=0;i<this->matrix_all_options.size();i++){
        for(int j=0; j<this->matrix_all_options.at(i).size(); j++){
            cout << this->matrix_all_options.at(i).at(j);
        }
        cout <<endl;
    }
    //ret = send(new_socket , hello , strlen(hello) , 0 ); 

    return 1;
}