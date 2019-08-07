#include "index_vector_convertor.h"



index_vector_convertor::index_vector_convertor(char file_char)
{
	this->file_char = file_char;
}

void index_vector_convertor::intialize(file_object& fileobj)
{
	this->fileobj = fileobj;
    std::string msg = fileobj.get_scheme_msg();
	this->parser.intialize(msg);
}

std::vector<int> index_vector_convertor::index_to_vector_indexes(int index){
    std::vector<int> result; 

    // fill the array with 0 this->scheme_string.length() times 
    result.assign(this->parser.get_str_compress().length(), 0); 
    int sum = 1;
    for (int i = 0; i<this->parser.get_str_compress().length();i++){
        sum *= size_of_object_in_scheme(i);
    }

    for (int i = this->parser.get_str_compress().length()-1; i >= 0; i--)
    {
        sum = sum / size_of_object_in_scheme(i);
        result[i] = (int) index/sum;
        index -= result[i]*sum;
    }
    return result;
}


int index_vector_convertor::vector_indexes_to_index(std::vector<int> &vec)
{
    int result = 0;
    int sum = 1;
    for (int i = 0; i<vec.size();i++)
    {
        result += sum*vec[i];
        sum *= size_of_object_in_scheme(i);
    }
    return result;
}


int index_vector_convertor::size_of_object_in_scheme(int index)
{
    if(index < 0 || index >= this->parser.get_str_compress().length() ){
        throw std::invalid_argument("unexpected index");
    }
    if(this->parser.get_str_compress().at(index) != file_char){
        return this->parser.get_str_compress_size_at(index);
        //return this->matrix_all_options[index].size();
    }
    int file_index = 0;
    for (int i = 0; i <= index; ++i)
    {
        if (this->parser.get_str_compress().at(i) == this->file_char)
        {
            file_index++;
        }
    }
    // we start to count from 0, and file_index counts how many files were(the len)
    file_index--;
    std::vector<std::string> strs;
    split(this->fileobj.get_files_for_scheme(), strs, '#');
    return get_number_of_lines_in_file(strs[file_index]);
}

int index_vector_convertor::get_number_of_lines_in_file(std::string filename)
{
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile;
    myfile.open(filename);

    while (std::getline(myfile, line))
        ++number_of_lines;
    return number_of_lines;
}

size_t index_vector_convertor::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}