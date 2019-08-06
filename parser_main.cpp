#include "parser_main.h"

parser_main::parser_main(std::string str)
{
    this->str_regular_expression = str;
    this->param_size = str.length();
    this->matrix_all_options = std::vector< std::vector<std::string> >(this->param_size);
    int retval = from_parser_string_to_matrix();
    if(retval == -1)
    {
        std::cout << "error in from_parser_string_to_matrix, returns -1" << std::endl;
    }
    update_compress_scheme_str();
}

std::vector<std::string> parser_main::from_known_char_to_options_list(char c, int* error_indicator)
{
    std::vector<std::string> result;
    *error_indicator = 1;
    switch(c)
    {

        case 'c' :
            result = std::vector<std::string>({"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"});
            break;
        case 'C':
            result = std::vector<std::string>({"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"});
            break;
        case 'p' :
            result = std::vector<std::string>({'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'});
            break;
        case 'd':
            result = std::vector<std::string>({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"});
            break;
        case 'f': // file
            result = std::vector<std::string>();
            break;
        default:
            result = std::vector<std::string>({""});
            *error_indicator = -1;
            break;
    }
    return result;
}

int parser_main::from_parser_string_to_matrix()
{
    int error_indicator = 0;
    for (int i = 0; i< this->param_size; i++){
        error_indicator = 0;
        this->matrix_all_options.at(i) = this->from_known_char_to_options_list(this->str_regular_expression.at(i), &error_indicator);
        if(error_indicator == -1){
            return -1;
        }
    }
    return 1;
}
std::vector<std::vector<std::string>>& parser_main::get_matrix()
{
    return this->matrix_all_options;
}

void parser_main::update_compress_scheme_str()
{
    this->str_compress = "";
    bool is_char = false;
    int element_size = 0;
    for (int i = 0; i < this.str_regular_expression.length(); i++)
    {
        if (this->str_regular_expression.at(i) != 'f')
        {
            element_size = element_size + this.matrix_all_options.at(i).size();
            if(! is_char)
            {
                is_char = true;
                this->str_compress = this->str_compress + 'C';        
            }
        }
        else
        {
            if (element_size != 0)
            {
                this->str_compress_size.push_back(element_size);
            }
            is_char = false;
            this->str_compress = this->str_compress + 'f';
            element_size = 0;

            // we dont get the file size from here
            this->str_compress_size.push_back(-1);
        }
    }
    if (element_size != 0)
    {
        this->str_compress_size.push_back(element_size);
    }
}

std::vector<int> parser_main::get_str_compress_size()
{
    return this->str_compress_size;
}
std::string parser_main::get_str_compress()
{
    return this->str_compress;
}
int parser_main::get_full_matrix_length()
{
    return this->matrix_all_options.size();
}
int parser_main::get_str_compress_size_at(int index)
{
    return this->str_compress_size.at(index);
}
std::string parser_main::get_str_original()
{
    return this->str_regular_expression;
}   