

#ifndef TAU_PROJECT_WORKER_MANAGER_H
#define TAU_PROJECT_WORKER_MANAGER_H
#include <iostream>
#include <string>
#include "parser_main.h"


using namespace std;
/**
 * This class is the main for the worker manager
 * Each worker asks the worker manager for the next job, and the manager organize all the worker job
 */

class worker_manager {

    vector< vector<string> > matrix_all_options;
    int sum_of_works;
    int current_index_of_job_in_progress;
    int index_of_job_make_sure_thaat_made; // not use in that section
    int job_size;
public:
    /**
     * constructor
     * @param str- the matrix from the parser with all the options
     */
    worker_manager(vector< vector<string> > matrix_all_options);

  
    /*
    * this function gives a starter
    */
    vector<vector<string>> intialize_worker();
    

};


#endif //TAU_PROJECT_PARSER_MAIN_H