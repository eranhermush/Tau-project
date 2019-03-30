//
// Created by Eran on 30/03/2019.
//

#include "parser_token.h"
parser_token::parser_token(int size_argument, char represented_char_argument)
{
    size  = size_argument;
    represented_char = represented_char_argument;
}

int parser_token::get_size()
{
    return size;
}


char parser_token::get_represented_char(){
    return represented_char;
}