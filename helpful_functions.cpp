#include "helpful_functions.h"


 
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

void helpful_functions::my_print(std::vector<std::string> &input)
{
	std::cout << "size: " << input.size() << std::endl;
    for (int i = 0; i < input.size(); i++) {
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


bool helpful_functions::get_next_int(std::string &str, int start_index, int size, int &result)
{
	result = 0;
	std::string current_working;
	if (size < 1)
	{
		return false;
	}
	if (start_index + size > str.length())
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

	int index = 0;
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
			get_int_result = helpful_functions::get_next_int(server_str, index, 1, counter);
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