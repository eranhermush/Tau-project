#include "worker.h"

worker_manager::worker(vector< vector<string> > matrix_all_options, int id, string server_ip, int server_port, int job_size)
{
	this->server_port = server_port;
	this->server_ip = server_ip;
    this->id = id;
    this->matrix_all_options = matrix_all_options;
    this->socket_to_server = -1;
    this->job_size = job_size;
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

int worker::work(int index){
	vector<int> lengths(size(this->matrix_all_options), 0);
	for(int i = 0; i < size(lengths); i++){
		lengths[i] = size(this->matrix_all_options[i]);
	}
	vector<int> indices = index_vector_from_index(index);
	string pass = password_from_indices_vector(indices);

	/* Just for now, use 3-caesar_cipher */

	for(int j = 0; j < job_size; j++){
		if(scheme::caesar_cipher(pass, 3) == this->target){
			// found an answer
			return j+index;
		}
		worker::advance_password(&pass, &indices, &lengths);
	}

	// no password was found
	return -1;
}

void worker::advance_password(string& pass, vector<int>& indices, vector<int>& lengths){
	int len = size(lengths);
	for(int i = 0; i < len; i++){
		indices[i]++;
		if(indices[i] < lengths[i]){
			pass[i] = this->matrix_all_options[i][indices[i]];
			return;
		}
		else{
			indices[i] = 0;
			pass[i] = this->matrix_all_options[i][indices[i]];
		}
	}
}

vector<int> worker::indices_vector_from_index(int index){
	// find the index of each charater
	int len = size(this->matrix_all_options);
	vector<int> indices(len, 0);
	for(int i = 0; i < len && index > 0; i++){
		indices[i] = index % size(this->matrix_all_options[i]);
		index /= size(this->matrix_all_options[i]);
	}
	return indices;
}

string worker::password_from_indices_vector(vector<int> indices){
	// find the index of each charater
	int len = size(indices);
	// fill with characters
	string password(len, '\0');
	for(int i = 0; i < len; i++){
		password[i] = this->matrix_all_options[i][indices[i]];
	}
	return password;
}