

#ifndef TAU_PROJECT_WORKER_H
#define TAU_PROJECT_WORKER_H
#include <iostream>
#include <string>

using namespace std;

#include <netdb.h> 


#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h> 
//#include <sys/socket.h> 
//#include <thread>

#include <sys/types.h> 
#include "worker_manager_worker_object.h"
#include "parser_main.h"


/**
 * This class is the main for the worker manager
 * Each worker asks the worker manager for the next job, and the manager organize all the worker job
 */

class worker_manager{

    vector< vector<string> > matrix_all_options;
    int sum_of_works;
    int current_index_of_job_in_progress;
    int index_of_job_make_sure_thaat_made; // not use in that section
    int job_size;
    int server_port;
    vector<worker_manager_worker_object> workers;
    int main_socket; 
public:
    /**
     * constructor
     * @param str- the matrix from the parser with all the options
     * @param server_port is the port of the worker manager
     */
    worker_manager(vector< vector<string> > &matrix_all_options, int server_port);

  
    /*
    * this function gives a starter worker the matrix of the work, and than the worker should go to "g"
    */
    vector<vector<string>> intialize_worker();

    /*
    * This function gets a worker id and returns a job for this worker, i.e. it returns the current index of the working
    * We assume that the workers do get_job_size job (they should know it by calling to get_size)
    *
    */
    int get_job(int id);

    /*
     * This function return the current job size for the workers
     * It is equivalent to getter for job_size
    */
    int get_job_size();

    /*
    * This function connect the server to the world :)
    * it makes the 'connect' and the bind, and it waits for the clients
    * @ret: 1 on success, -1 on error
    */
    int create_server_of_worker_manager();

    /*
    * This function is the thread function: each client that connects to the server, has a thread
    * This thread listen to its calls and handles them

    */
    int worker_handler_function(int socket);


    /*
        This function is the main for the manager, it creates here the worker and gives them jobs
    */
    int main();
};


#endif //TAU_PROJECT_PARSER_MAIN_H