#include "file_password_generator.h"

File_Password_Generator::File_Password_Generator(const std::string& file_path, int first, int last, char delim):
	path(file_path), first_byte(first), last_byte(last), delimiter(delim){
		curr_password = "";
		init(first_byte);
	}

File_Password_Generator::File_Password_Generator(const File_Password_Generator& generator):
	path(generator.path), first_byte(generator.first_byte), last_byte(generator.last_byte), delimiter(generator.delimiter){
		curr_password = generator.curr_password;
		init(generator.curr_position);
}

std::unique_ptr<Password_Generator> File_Password_Generator::clone() const{
	return std::unique_ptr<Password_Generator>(new File_Password_Generator(*this));
}

void File_Password_Generator::init(int file_offest){
	// open the file 
	password_file.open(path.c_str(), std::ifstream::in);
	// load the first password
	password_file.seekg(file_offest);
	curr_position = file_offest;
	advance_password();
}

void File_Password_Generator::advance_password(){
	if(has_next()){
		// curr_position is updated to the start of the new password
		curr_position = password_file.tellg();
		std::getline(password_file, curr_password, delimiter);
	}
	else{
		// finished with the last password
		is_over_flag = true;
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

bool File_Password_Generator::has_next() const{
	// error/eof means the generator cannot produce any more reliable passwords
	return password_file.good() && curr_position >= 0 && curr_position < last_byte;
}