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
