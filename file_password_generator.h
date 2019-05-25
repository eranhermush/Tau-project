#ifndef TAU_PROJECT_FILE_PASSWORD_GENERATOR_H
#define TAU_PROJECT_FILE_PASSWORD_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include "password_generator.h"

/* File_Password_Generator extracts paswords from a file. */

class File_Password_Generator: public Password_Generator{
	public:
		/* file_path is the path to a passwords file.
			first and last are positions in the file (byte offest from the beginning).
			delim is the delimiter used in the file, (newline by default)

			The first password generated would be the one starting at first (its beginning may be truncated)
			The last password generated would be the the last one starting before last (its ending can be after end)*/
		File_Password_Generator(const std::string file_path, int first, int last, char delim='\n');

		/* Advances the current password to the next one (if one exists in the range) */
		void advance_password();

		/* Returns whether the generator has more passwords in its range */
		bool has_next();

		/* Reverts to the first password */
		void reset();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length(){
			return false;
		}

		/* Returns whether a file error has occured (badbit is set) */
		bool file_error();

	private:
		std::string path;
		int first_byte;
		int last_byte;
		char delimiter;
		std::string curr_password;
		std::ifstream password_file;

		void init();
};


#endif