#include "worker.h"

worker_manager::worker(vector< vector<string> > matrix_all_options, int id, string server_ip, int server_port)
{
	this->server_port = server_port;
	this->server_ip = server_ip;
    this->id = id;
    this->matrix_all_options = matrix_all_options;
    this->socket_to_server = -1;
}

int worker::intialize_worker_client()
{
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(this->server_port); 
    serv_addr.sin_addr.s_addr = this->server_ip; 

   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    this->socket_to_server = sock;
    return 0;
}

int worker::get_a_job_from_server()
{
    // Todo
    return 0;
}