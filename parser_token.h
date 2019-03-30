//
// Created by Eran on 30/03/2019.
//

#ifndef TAU_PROJECT_PARSER_TOKEN_H
#define TAU_PROJECT_PARSER_TOKEN_H

/**
 * This class represent a token in he regular expression of the user input
 * it has two properties, the token's char (like 'c' or 'x') and the token's size.
 */
class parser_token {
    int size;
    char represented_char;
public:
    /**
     * constructor
     * @param size_argument
     * @param represented_char_argument
     */
    ParserObject(int size_argument, char represented_char_argument);
    /**
     * getter for the size
     * @return the size
     */
    int get_size();
    /**
     * getter for the char
     * @return the char of the regular expression
     */
    char get_represented_char();
};



#endif //TAU_PROJECT_PARSER_TOKEN_H
