#include "worker.h"



worker::worker()
{
	cout<<"hi :)"<<endl;
	this->server_port = 8811;
	this->server_ip = "127.0.0.1";
	this->target = target;
	this->job_size = 0;
	intialize_worker_client();

}

void worker::start_worker(vector< vector<string> > &matrix_all_options, int id, string server_ip, int server_port, int job_size)
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
    	perror("error: ");	
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr));
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(this->server_port);
    const char *const ip = this->server_ip.data();
    inet_aton(ip, reinterpret_cast<in_addr *>(&serv_addr.sin_addr.s_addr));
    //serv_addr.sin_addr.s_addr = this->server_ip;

   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
    	perror("error: ");	
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    cout << "connect" << endl;
    this->socket_to_server = sock;
    return 0;
}


int worker::get_matrix(){
	char user_message[1024] = {0};
	int valread = 0;
	valread = read(this->socket_to_server , user_message, 1024); 
	if (valread <0 ){
		perror("error get matrix:");
		return -1;
	}
	parser_main parser (user_message);
	
	// create the matrix
	parser.from_parser_string_to_matrix();
	//print_matrix(parser.get_matrix());

	// creates the manager
	this->matrix_all_options = parser.get_matrix();
	return 0;

}
int worker::send_message(string message)
{
    int ret = 0;
    ret = send(this->socket_to_server, message.c_str() , message.length(), 0 ); 
    if (ret <0){
        return -1;
    } 
    return 0;
}
int worker::get_target()
{
	char user_message[1024] = {0};
	int valread = 0;
	valread = read(this->socket_to_server , user_message, 1024); 
	if (valread <0 ){
		perror("error get target:");
		return -1;
	}
	this->target = user_message;
	return 0;
}

int worker::get_work_size()
{
	int ret = 0;
	unsigned int* val;
	ret = another_functions::receive_int(val, this->socket_to_server);

	
	if (ret <0 ){
		perror("error get size:");
		return -1;
	}
	this->job_size = *val;
	return 0;
}

int worker::get_index()
{
	int ret = 0;
	unsigned int* val;
	ret = another_functions::receive_int(val, this->socket_to_server);
	
	if (ret <0 ){
		perror("error get size:");
		return -1;
	}
	return *val;
}



string worker::work(int index){
	vector<int> lengths((this->matrix_all_options.size()), 0);
	for(int i = 0; i < (lengths.size()); i++){
		lengths[i] = this->matrix_all_options[i].size();
	}
	vector<int> indices = indices_vector_from_index(index);
	string pass = password_from_indices_vector(indices);

	/* Just for now, use 3-caesar_cipher */

	for(int j = 0; j < job_size; j++){
		if(scheme::caesar_cipher(pass, 3) == this->target){
			// found an answer
			//return j+index;
			return pass;
		}
		worker::advance_password(pass, indices, lengths);
	}

	// no password was found
	return "";
}

void worker::advance_password(string &pass, vector<int>& indices, vector<int>& lengths){
	int len = lengths.size();
    int current_index =0;
    pass = "";
    for(int i = 0; i < len; i++){
		indices[i]++;
		current_index = indices.at(i);
		if(indices.at(i) < lengths[i]){
			pass =pass + this->matrix_all_options.at(i).at(current_index);
			return;
		}
		else{
			indices[i] = 0;
			pass = pass + this->matrix_all_options.at(i).at(current_index);
		}
	}
}

vector<int> worker::indices_vector_from_index(int index){
	// find the index of each charater
	int len = this->matrix_all_options.size();
	vector<int> indices(len, 0);
	for(int i = 0; i < len && index > 0; i++){
		indices[i] = index % this->matrix_all_options[i].size();
		index /= this->matrix_all_options[i].size();
	}
	return indices;
}

string worker::password_from_indices_vector(vector<int> indices){

	// find the index of each charater
	int len = indices.size();
	// fill with characters
	string password(len, '\0');
	int current_index = 0;
	for(int i = 0; i < len; i++){
		current_index = indices.at(i);
	    password =password + this->matrix_all_options.at(i).at(current_index);
	}
	return password;

}