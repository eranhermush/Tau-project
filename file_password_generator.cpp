#include "file_password_generator.h"

File_Password_Generator::File_Password_Generator(std::string file_path, int first, int last, char delim):
	path(file_path), first_byte(first), last_byte(last), delimiter(delim){
		curr_password = "";
		init();
	}

void File_Password_Generator::init(){
	// open the file 
	password_file.open(path.c_str(), std::ifstream::in);
	password_file.seekg(first_byte);
	// load the first password
	advance_password();
}

void File_Password_Generator::advance_password(){
	if(has_next()){
		std::getline(password_file, curr_password, delimiter);
	}
}

void File_Password_Generator::reset(){
	password_file.seekg(first_byte);
	// load the first password
	advance_password();
}

bool File_Password_Generator::file_error(){
	return password_file.bad();
}

bool File_Password_Generator::has_next(){
	int pos = password_file.tellg();
	// error/eof means the generator cannot produce any more reliable passwords
	return password_file.good() && pos >= 0 && pos < last_byte;
}