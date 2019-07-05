#include "parser_main.h"

parser_main::parser_main(string str)
{
    this->str_regular_expression = str;
    this->param_size = str.length();
    this->matrix_all_options = vector< vector<string> >(this->param_size);
}

vector<string> parser_main::from_known_char_to_options_list(char c, int* error_indicator)
{
    vector<string> result;
    *error_indicator = 1;
    switch(c)
    {
        case 'c' :
            result = vector<string>({"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"});
            break;
        case 'C':
            result = vector<string>({"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"});
            break;
        case 'd':
            result = vector<string>({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"});
            break;
        case 'f': // file
            result = vector<string>();
            break;
        default:
            result = vector<string>({""});
            *error_indicator = -1;
            break;
    }
    return result;
}

int parser_main::from_parser_string_to_matrix(){
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
vector<vector<string>>& parser_main::get_matrix(){
    return this->matrix_all_options;
}