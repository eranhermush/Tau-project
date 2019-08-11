#include "helpful_functions.h"

uint64_t helpful_functions::get_file_size(const std::string& path){
	std::ifstream file(path, std::ifstream::in | std::ifstream::ate);
	if(file.bad() || !file.is_open()){
		return 0;
	}
	int64_t size = file.tellg();
	if(size <= 0){
		// cannot open or simply an empty file
		return 0;
	}
	return(uint64_t) size;
}

// Using c++11 so it seem the C solution is the best available
// https://stackoverflow.com/questions/4109638/what-is-the-safe-alternative-to-realpath
std::string helpful_functions::get_absolute_path(const std::string& rel_path){
	char* abs_path;
	char* abs_path2;
	std::string absolute_path;
	abs_path = realpath(rel_path.c_str(), NULL);
	abs_path2 = abs_path;
	if(abs_path){
		abs_path2 = realpath(rel_path.c_str(), abs_path);
		absolute_path = abs_path;
		free(abs_path);
	}
	return absolute_path;
}
 
void helpful_functions::read_directory(std::string& name,  std::vector<std::string> &v)
{
    DIR* dirp = opendir(name.c_str());
    if (! dirp)
    {
    	perror("error in opendir");
    	return;
    }
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
    	if (! ( (strcmp(dp->d_name,".") == 0 ) || (strcmp(dp->d_name,"..") == 0 ) ) )
    	{
        	v.push_back(dp->d_name);
        }
    }
    closedir(dirp);
}
int helpful_functions::index_of_file_object_to_fileindex(std::string& str, int index)
{
	int realIndex = 0;
	int str_length = str.length();
	for (int i = 0; i < index && i < str_length; ++i)
	{
		if(str[i] == 'f')
		{
			realIndex++;
		}
	}
	return realIndex;
}

void helpful_functions::my_print(std::vector<std::string> &input)
{
	std::cout << "size: " << input.size() << std::endl;
	int input_size = input.size();
    for (int i = 0; i < input_size; i++) {
        std::cout << input.at(i) << ' ';
    }
    std::cout << std::endl;
}
bool helpful_functions::is_number(std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//013444114987612103333333333
bool helpful_functions::get_next_int(std::string &str, int start_index, int size, int &result)
{
	result = 0;
	std::string current_working;
	int str_length = str.length();
	if (size < 1)
	{
		return false;
	}
	if (start_index + size > str_length)
	{
		return false;
	}
	current_working = str.substr(start_index, size);
	if (! is_number(current_working))
	{
		return false;
	}
	
	result = std::stoi(current_working);

	return true;
}
bool helpful_functions::isFloat(std::string &s)
{
    std::size_t char_pos(0);

    // check the significand
    if (s[char_pos] == '+' || s[char_pos] == '-') ++char_pos; // skip the sign if exist

    int n_nm, n_pt;
    for (n_nm = 0, n_pt = 0; std::isdigit(s[char_pos]) || s[char_pos] == '.'; ++char_pos) {
        s[char_pos] == '.' ? ++n_pt : ++n_nm;
    }
    if (n_pt>1 || n_nm<1) // no more than one point, at least one digit
        return false;

    // skip the trailing whitespaces
    while (s[char_pos] == ' ') {
        ++ char_pos;
    }

    return char_pos == s.size();  // must reach the ending 0 of the string
}


bool helpful_functions::server_string_to_vectors(std::string& server_str, std::vector<std::string> &str_vec, std::vector<int> &int_vec,std::vector<float> &float_vec)
{

	str_vec.clear();
	int_vec.clear();
	float_vec.clear();
	if (server_str == "-1")
	{
		return true;
	}
	unsigned int index = 0;
	int x, d1, d2, counter;
	float fcounter;
	std::string current_working;
	bool get_int_result;
	while (index < server_str.length())
	{
		get_int_result = helpful_functions::get_next_int(server_str, index, 1, x);
		if (! get_int_result)
		{
			return false;
		}
		index++;

		get_int_result = helpful_functions::get_next_int(server_str, index, 1, d1);
		if (! get_int_result)
		{
			return false;
		}
		index++;

		get_int_result = helpful_functions::get_next_int(server_str, index, d1, d2);
		if (! get_int_result)
		{
			return false;
		}
		index = index + d1;

		if (index + d2 > server_str.length())
		{
			return false;
		}
		current_working = server_str.substr(index, d2);

		// string
		if (x == 0)
		{
			str_vec.push_back(current_working);
		}
		if (x == 1)
		{
			get_int_result = helpful_functions::get_next_int(server_str, index, d2, counter);
			if (! get_int_result)
			{
				return false;
			}
			int_vec.push_back(counter);
		}
		if (x == 2)
		{
			if (! helpful_functions::isFloat(current_working))
			{
				return false;
			}
			fcounter = std::stof(current_working);
			float_vec.push_back(fcounter);
		}
		index = index + d2;
	
	}
	return true;
}

bool helpful_functions::write_data_to_file(std::string& dir, std::string filename, const std::string& data_to_file)
{
    std::ofstream myfile;
    // write the data without the status (write status 2)
    std::string path =  dir + "/" + filename + ".txt";
    //std::string path = std::to_string(worker_id) + ".txt";
    myfile.open(path, std::fstream::out | std::fstream::trunc);
    
    if (! (myfile.is_open()))
    {
        std::cout << "Error opening file in write_work_to_file" << std::endl;
        return false;
    }
    myfile << data_to_file;
    myfile.flush();
    myfile.close();

    return true;
}


int helpful_functions::file_to_file_object(file_object& file_obj, std::string filename, bool print_error)
{
    std::string line, line2;
    std::string msg;
    std::string files = "";
    std::ifstream myfile (filename);
    std::istringstream start_stream, finish_stream;
    uint64_t start_index_value, finish_index_value;
    int status = 0;
    int len = 0;
    /*
        std::string result = std::to_string(this->id) + '\n' + std::to_string(this->worker_id) + '\n' + this->scheme_msg+ '\n' + this->password_function + 
        '\n' + std::to_string(this->start_index) + '\n' + std::to_string(this->end_index)+ '\n' + this->files_for_scheme + '\n' + this->passwords;
    */

    if (myfile.is_open())
    {
        getline (myfile,line);
        //std::cout << "before " << line << std::endl;
        status = std::stoi(line);
        //std::cout << "after " << line << std::endl;
        file_obj.set_status(status);

        getline (myfile,line);
        file_obj.set_id(std::stoi(line));

        getline (myfile,line);
        file_obj.set_worker_id(std::stoi(line));
        if(status == file_object::start_worker_symbol)
        {
        	myfile.close();
        	return 0;
        } 

        //std::cout << "status = " << status << " " << file_obj.get_worker_id() << " " << file_obj.get_id() << std::endl;       
        if( status != file_object::worker_found_password_symbol)
        {
        	getline (myfile,line);
	        file_obj.set_scheme_msg(line);
	        msg = line;
	        getline (myfile,line);
	        file_obj.set_password_function(line);
	        getline (myfile,line);
	        getline (myfile,line2);

	        start_stream = std::istringstream(line);
	        finish_stream = std::istringstream(line2);

	        start_stream >> start_index_value;
	        finish_stream >> finish_index_value;
	        file_obj.set_index(start_index_value, finish_index_value);

	        for (int i = 1; i <= std::count(msg.begin(), msg.end(),'f'); i++)
	        {
	            getline (myfile,line);
	            files = files + line + '\n'; 
	        }
	        file_obj.set_files_for_scheme(files);
	        getline (myfile,line);
	        file_obj.set_passwords(line);
	        getline (myfile,line);
	        file_obj.set_arguments(line);
	    }
	    else
	    {
	    	getline (myfile,line);
	    	//std::cout << "line = " << line << std::endl;       

	    	len = std::stoi(line);
	    	for (int i = 0; i < len; ++i)
	    	{
	    		getline (myfile,line);
	    		file_obj.add_password_found_vector(line);
	    	}
	    }
        myfile.close();
    }
    else
    {
        if (print_error)
        {
            std::cout << "Error my file is not open :( file name is " << filename << std::endl;
        }
        return -1;
    }
    return 0;   
}

bool helpful_functions::change_status_of_file(std::string& path, int status)
{
	FILE* fp;
	if(status < 0 || status > 9)
	{
		return false;
	}
	fp = std::fopen(path.c_str(),"r+");
    fseek(fp, 0, SEEK_SET);
    if (fp == NULL) {
        perror("Error fopen in write_work_to_file ");
        return false;
    }
    fprintf(fp, "%s", std::to_string(status).c_str());
    std::fclose (fp);
    return true;
}

size_t helpful_functions::split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}