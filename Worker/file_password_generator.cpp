#include "file_password_generator.h"

File_Password_Generator::File_Password_Generator(const std::string& file_path, uint64_t first, uint64_t last, char delim):
	path(file_path), first_byte(first), last_byte(last), delimiter(delim){
		curr_password = "";
		init(first_byte);
	}

File_Password_Generator::File_Password_Generator(const File_Password_Generator& generator):
	path(generator.path), first_byte(generator.first_byte), last_byte(generator.last_byte), delimiter(generator.delimiter){
		curr_password = generator.curr_password;
		is_over_flag = generator.is_over_flag;
		init(generator.curr_position);
}

std::unique_ptr<Password_Generator> File_Password_Generator::clone() const{
	return std::unique_ptr<Password_Generator>(new File_Password_Generator(*this));
}

void File_Password_Generator::init(uint64_t file_offest){
	// open the file at the end to get file size
	password_file.open(path.c_str(), std::ifstream::in | std::ifstream::ate);
	if(!password_file.good()){
		zero_params_on_file_error();
		file_init_error = true;
		return;
	}
	// get file size in bytes
	int64_t size = password_file.tellg();
	if(size <= 0){
		// error in getting size or file is empty so there are no passwords
		zero_params_on_file_error();
		file_init_error = true;
		return;
	}
	file_size = (uint64_t) size;
	// make sure the ranges make sense
	first_byte = first_byte < file_size - 1 ? first_byte : file_size - 1; // first_byte is no larger than file size
	last_byte = last_byte > first_byte ? last_byte : first_byte; // last_byte at least than first_byte
	last_byte = last_byte < file_size - 1 ? last_byte : file_size - 1; // last_byte is no larger than the last byte of the file
	file_offest = file_offest < last_byte ? file_offest : last_byte; // file_offest is in range
	file_offest = file_offest > first_byte ? file_offest : first_byte; // file_offest is in range
	// load the first password
	password_file.seekg(file_offest);
	curr_position = file_offest;
	advance_password();
}

void File_Password_Generator::advance_password(){
	if(has_next()){
		// curr_position is updated to the start of the new password
		int64_t pos = password_file.tellg();
		if(pos < 0){
			zero_params_on_file_error();
			file_init_error = true;
			return;
		}
		curr_position = (uint64_t) pos;
		std::getline(password_file, curr_password, delimiter);
	}
	else{
		if(password_file.bad()){
			zero_params_on_file_error();
		}
		// finished with the last password
		is_over_flag = true;
	}
}

void File_Password_Generator::set_password(uint64_t position){
	if(position > last_byte || position < first_byte){
		return;
	}
	Password_Generator::is_over_flag = false;
	password_file.seekg(position);
	// curr_position is updated in advance_password, but has next needs to pass has_next
	// this is a safe valid placeholder value
	curr_position = first_byte;
	advance_password();

}

void File_Password_Generator::reset(){
	set_password(first_byte);
}

void File_Password_Generator::expand_to_max_range(){
	first_byte = 0;
	if(last_byte < file_size - 1){
		Password_Generator::is_over_flag = false;
		last_byte = file_size - 1;
	}
}

bool File_Password_Generator::file_error() const{
	return password_file.bad() || file_init_error;
}

bool File_Password_Generator::has_next() const{
	// error/eof means the generator cannot produce any more reliable passwords
	return password_file.good() && !file_init_error && curr_position < last_byte;
}

void File_Password_Generator::zero_params_on_file_error(){
	file_size = 1;
	first_byte = 0;
	last_byte = 0;
	curr_position = 0;
	is_over_flag = true;
}