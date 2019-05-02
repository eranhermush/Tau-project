#ifndef TAU_PROJECT_WORKER_H
#define TAU_PROJECT_WORKER_H
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "scheme.h"

using namespace std;
/**
 * This class is the worker class 
 * it represents each worker, and has functions gives a request for new job, and functions that goes over all the options
 */

class worker {

    int worker_id;
    vector< vector<string> > matrix_all_options;
    string server_ip;
    string target;
    int server_port;
    int job_size;
    int id;
    int socket_to_server;
public:
    /*
        constructor
    */
    worker();
    /**
     * @param str- the matrix from the parser with all the options
     * @param id- the worker id
     * @param server_ip is the ip of the worker manager
     * @param server_port is the port of the worker manager
     * @param job_size - the size of each working iteration
     */
    void start_worker(vector<vector<string>> &matrix_all_options, int id, string server_ip, int server_port, int job_size);

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
  
    /* 
    * This function checks the all strings in the range [index, index + job_size),
    * if any match the target (with the caesar cipher), returns its index.
    * Otherwise, -1 is returned.
    */
    int work(int index);

    /*
    * Advances a password and its indices with the format of matrix_all_options by 1.
    * @param pass - a refrence to a current password
    * @param indices - a refrence to a mathcing indiced vector
    * @parma lengths - a refrence to the range length of each coordinate
    */
    void advance_password(string& pass, vector<int>& indices, vector<int>& lengths);

    /*
    * Returns a vector of indices from the given index, according to matrix_all_options.
    */
    vector<int> indices_vector_from_index(int index);

    /*
    * Returns the string matching the indices vector, according to matrix_all_options.
    */
    string password_from_indices_vector(vector<int> indices);
};


#endif //TAU_PROJECT_WORKER_H