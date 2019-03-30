//
// Created by Eran on 30/03/2019.
//

#include "parser_main.h"

parser_main::parser_main(str)
{
    str_regular_expression = str;
}

Vector<char> & parser_main::from_known_char_to_options_list(char c)
{
    char result[] = {};
    switch(c)
    {
        case 'c' :
            result = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
            break;
        case 'C':
            result = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
            break;
        case 'd':
            result = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
            break;
    }
    return vector<char> (myints, myints + sizeof(myints) / sizeof(int) );
}