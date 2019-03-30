//
// Created by Eran on 30/03/2019.
//

#ifndef TAU_PROJECT_PARSER_MAIN_H
#define TAU_PROJECT_PARSER_MAIN_H

/**
 * This class is the main for the parser
 * it has functions that convert the regular expression string to array of tokens
 * it also checks the string
 */
using namespace std;

class parser_main {
    string str_regular_expression;
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
    Vector<char>& from_known_char_to_options_list(char c);

};


#endif //TAU_PROJECT_PARSER_MAIN_H
