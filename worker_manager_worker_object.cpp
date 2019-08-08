#include "worker_manager_worker_object.h"

worker_manager_worker_object::worker_manager_worker_object(int id, int socket){
	this->worker_id = id;
	this->worker_socket = socket;
	//this->worker_thread = move(th);
}
int worker_manager_worker_object::get_socket(){
	return this->worker_socket;
}