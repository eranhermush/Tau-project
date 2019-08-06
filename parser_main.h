#ifndef TAU_PROJECT_PARSER_MAIN_H
#define TAU_PROJECT_PARSER_MAIN_H
#include <iostream>
#include <string>
#include <vector>
/**
 * This class is the main for the parser
 * it has functions that convert the regular expression string to array of tokens
 * it also checks the string
 */

class parser_main {
    std::string str_regular_expression;
    int param_size;
    std::vector< std::vector<std::string> > matrix_all_options;

    // the compression mode
    // it saves the size of the compress item, i.e. for str = "cCdfdcd", str_compress_size = Cfc and str_compress_size = 62, -1, 36
    std::vector<int> str_compress_size;
    std::string str_compress;

public:

    /**
     * constructor
     * @param str- the regular expression string
     */
    parser_main(std::string str);

    /**
    * convert regular expression char to its array of values
    * @param c: the known char of the regular expression, like 'c'
    * @return  The array of option for this char, or empty vector for unknown char
    * @return error_indicator- an int that the function uses to return if there was an error (-1 is error)
    */
    std::vector<std::string> from_known_char_to_options_list(char c, int* error_indicator);  

    /**
    *    convert the parser string to the parser matrix
    *    assusme that the parser_main object is intialized
    *    the function returns -1 if there was an error (i.e. an invalid char)
    */     
    int from_parser_string_to_matrix();    
    /*
    * getter for the matrix
    * we assume that the matrix is intialized
    */
    std::vector<std::vector<std::string> >& get_matrix();

    /*
    * this function updates the sting to the format of the Char_Pattern_Password_Generator format
    * for example: cccc -> C, ccfcCCf -> CfCf
    * we compress the chars (non files);
    * this function also updates the size vector, str_compress_size
    */
    void update_compress_scheme_str();

    /*
    * some getters 
    */
    std::vector<int> get_str_compress_size();
    int get_str_compress_size_at(int index);
    std::string get_str_compress();
    std::string get_str_original();

    int get_full_matrix_length();

};

#endif //TAU_PROJECT_PARSER_MAIN_H