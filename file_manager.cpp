#include "file_manager.h"

file_manager::file_manager(std::vector< std::vector<std::string> > &matrix_all_options, std::vector<std::string> &file_names, std::string &scheme_string)
{
    this->matrix_all_options = matrix_all_options;
    this->file_names = file_names;
    this->scheme_string = scheme_string;
    validate_input();
}

int file_manager::vector_indexes_to_index(std::vector<int> &vec)
{
    int result = 0;
    int sum = 1;
    for (int i = 0; i<vec.size() ;i++){
        result += sum*vec[i];
        sum *= this->matrix_all_options[i].size();
    }
    return result;
}

bool file_manager::validate_input()
{
    if(this->scheme_string.length() <=0)
    {
        throw std::invalid_argument("received an empty string");
    }
    if(this->scheme_string.length() != this->matrix_all_options.size())
    {
        throw std::invalid_argument("received different sizes for the matrix and the string");
    }
    if(std::count(this->scheme_string.begin(), this->scheme_string.end(), 'f') != this->file_names.size())
    {
        throw std::invalid_argument("received different sizes for the file names and the number of files in the scheme");
    }
    return true;
}

int file_manager::size_of_object_in_scheme(int index){

}