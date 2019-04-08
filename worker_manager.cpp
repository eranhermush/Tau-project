

#include "worker_manager.h"

worker_manager::worker_manager(vector< vector<string> > matrix_all_options)
{
    this->matrix_all_options = matrix_all_options;
    this->sum_of_works = 0;
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
vector<vector<string>> intialize_worker(){
	return this->matrix_all_options;
}
