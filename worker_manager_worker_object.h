
#ifndef TAU_WORKER_MANAGER_WORKER_OBJECT_H
#define TAU_WORKER_MANAGER_WORKER_OBJECT_H

#include <iostream>
#include <string>
//#include <thread>
using namespace std;

/**
 * This class represents a worker in the worker_manager view
 * it contains the worker id, the worker thread with the manager and the worker socket
 */

class worker_manager_worker_object {
	int worker_id, worker_socket;
	//thread worker_thread;

public:
    /**
     * constructor
     * @param: id, socket, thread
     */
     worker_manager_worker_object(int id, int socket);
};
#endif //TAU_WORKER_MANAGER_WORKER_OBJECT_H