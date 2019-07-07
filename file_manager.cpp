#include "file_manager.h"

file_manager::file_manager(std::vector< std::vector<std::string> > &matrix_all_options, std::vector<std::string> &file_names, std::string &scheme_string)
{
    this->matrix_all_options = matrix_all_options;
    this->file_names = file_names;
    this->scheme_string = scheme_string;
    this->file_char = 'f';
    validate_input();
}

int file_manager::vector_indexes_to_index(std::vector<int> &vec)
{
    int result = 0;
    int sum = 1;
    for (int i = 0; i<vec.size();i++){
        result += sum*vec[i];
        sum *= size_of_object_in_scheme(i);
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
    if(std::count(this->scheme_string.begin(), this->scheme_string.end(), this->file_char) != this->file_names.size())
    {
        throw std::invalid_argument("received different sizes for the file names and the number of files in the scheme");
    }
    return true;
}

int file_manager::size_of_object_in_scheme(int index)
{
    if(index <0 || index >= this->scheme_string.length()){
        throw std::invalid_argument("unexpected index");
    }
    if(this->scheme_string.at(index) != file_char){
        return this->matrix_all_options[index].size();
    }
    int file_index = 0;
    for (int i = 0; i <= index; ++i)
    {
        if (this->scheme_string.at(i) == this->file_char)
        {
            file_index++;
        }
    }
    // we start to count from 0, and file_index counts how many files were(the len)
    file_index--;
    return Get_number_of_lines_in_file(this->file_names[file_index]);
}
int file_manager::Get_number_of_lines_in_file(std::string filename)
{
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile;
    myfile.open(filename);

    while (std::getline(myfile, line))
        ++number_of_lines;
    return number_of_lines;
}

std::vector<int> file_manager::index_to_vector_indexes(int index){
    std::vector<int> result; 

    // fill the array with 0 this->scheme_string.length() times 
    result.assign(this->scheme_string.length(), 0); 
    int sum = 1;
    for (int i = 0; i<this->scheme_string.length();i++){
        sum *= size_of_object_in_scheme(i);
    }

    for (int i = this->scheme_string.length()-1; i >= 0; i--)
    {
        sum = sum / size_of_object_in_scheme(i);
        result[i] = (int) index/sum;
        index -= result[i]*sum;
    }
    return result;
}
