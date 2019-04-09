

#ifndef TAU_PROJECT_WORKER_MANAGER_H
#define TAU_PROJECT_WORKER_MANAGER_H
#include <iostream>
#include <string>
#include "worker_manager.h"


using namespace std;
/**
 * This class is the worker class
 * it represents each worker, and has functions gives a request for new job, and functions that goes over all the options
 */

class worker {

    int worker_id;
    vector< vector<string> > matrix_all_options;
    string server_ip;
    int server_port;
    int socket_to_server;
public:
    /**
     * constructor
     * @param str- the matrix from the parser with all the options
     * @param id- the worker id
     * @param server_ip is the ip of the worker manager
     * @param server_port is the port of the worker manager
     */
    worker(vector< vector<string> > matrix_all_options, int id, string server_ip, int server_port);

    /*
    * this function intialize the worker as a client of the server
    * it connect to the server, and save the socket at this->socket_to_server
    * it returns 0 on success, -1 when there was an error
    */
    int intialize_worker_client();
    /*
    * Thsi function gets a job from server, it sends a request to the server for a job
    * it gets the job
    * it returns the job index if seccess, otherwise -1
    */
    int get_a_job_from_server();
  


};


#endif //TAU_PROJECT_PARSER_WORKER_H