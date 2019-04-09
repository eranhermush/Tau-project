//
// Created by Eran on 30/03/2019.
//

#ifndef TAU_PROJECT_PARSER_MAIN_H
#define TAU_PROJECT_PARSER_MAIN_H
#include <iostream>
#include <string>
using namespace std;
/**
 * This class is the main for the parser
 * it has functions that convert the regular expression string to array of tokens
 * it also checks the string
 */
using namespace std;

class parser_main {
    string str_regular_expression;
    int param_size;
    vector< vector<string> > matrix_all_options;

public:
    /**
     * constructor
     * @param str- the regular expression string
     */
    parser_main(string str);

    /**
     * convert regular expression char to its array of values
     * @param c: the known char of the regular expression, like 'c'
     * @return  The array of option for this char, or empty vector for unknown char
     */
    vector<char> from_known_char_to_options_list(char c);  

    /**
        convert the parser string to the parser matrix
        assusme that the parser_main object is intialized
    */     
    void from_parser_string_to_matrix();    
    /*
    getter for the matrix
    we assume that the matrix is intialized
    */
    <vector<vector<string>> get_matrix();


};


#endif //TAU_PROJECT_PARSER_MAIN_H