#include "file_manager.h"

file_manager::file_manager(std::vector< std::vector<std::string> > &matrix_all_options){
    this->matrix_all_options = matrix_all_options;
}

int vector_indexes_to_index(std::vector<int> &vec){
    int result = 0;
    int sum = 1;
    for (int i = 0; i<vec.size() ;i++){
        result += sum*vec[i];
        sum *= this->matrix_all_options[i].size();
    }
    return result;
}

int scheme_at_index_to_len(int index){
    return -1;
}
