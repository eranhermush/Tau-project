#ifndef TAU_PROJECT_FILE_PASSWORD_GENERATOR_H
#define TAU_PROJECT_FILE_PASSWORD_GENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include "password_generator.h"

/* File_Password_Generator extracts paswords from a file. */

class File_Password_Generator: public Password_Generator{
	public:
		/* file_path is the path to a passwords file.
			first and last are positions in the file (byte offest from the beginning).
			delim is the delimiter used in the file, (newline by default)

			The first password generated would be the one starting at first (its beginning may be truncated)
			The last password generated would be the the last one starting before last (its ending can be after end)*/
		File_Password_Generator(const std::string& file_path, uint64_t first, uint64_t last, char delim='\n');

		/* Copy constructor - also copies the current password and the current file object */
		File_Password_Generator(const File_Password_Generator& generator);

		/* Returns a unique pointer to a new clone of the generator */
		std::unique_ptr<Password_Generator> clone() const;

		/* Advances the current password to the next one (if one exists in the range) */
		void advance_password();

		/* Sets the current password to be the one starting at file offest postition (position should be in the range)*/
		void set_password(uint64_t position);

		/* Returns whether the generator has more passwords in its range */
		bool has_next() const;

		/* Reverts to the first password */
		void reset();

		/* Expands the password range to the widest possible of the generator (the entire file)
			Does not change the current password */
		void expand_to_max_range();

		/* Returns whether the generator produces passwords of fixed length */
		bool has_fixed_length() const{
			return false;
		}

		uint64_t get_first_position() const{
			return first_byte;
		}

		uint64_t get_last_position() const{
			return last_byte;
		}

		uint64_t get_range_length() const{
			return last_byte - first_byte + 1;
		}

		uint64_t get_curr_position() const{
			return curr_position;
		}

		/* Returns whether a file error has occured (badbit is set) */
		bool file_error() const;

		/* Returns whether the generator has finished will all passwords (== advanced from the last password) */
		bool is_over() const{
			return Password_Generator::is_over_flag || file_error();
		}

	private:
		std::string path;
		uint64_t first_byte;
		uint64_t last_byte;
		uint64_t curr_position;
		uint64_t file_size;
		char delimiter;
		std::ifstream password_file;
		bool file_init_error = false;

		void init(uint64_t file_offest);
		void zero_params_on_file_error();
};


#endif