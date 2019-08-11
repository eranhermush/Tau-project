#include "client.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{	
	if(argc < 3){
		std::cerr << "Usage error: expecting 2 arguments: a path to the working directory and worker id number." << std::endl;
		return 1;
	}
	std::string dir(argv[1]);
	int id = std::stoi(argv[2]);
	client c(id, dir, 10000);
	c.main();

	return 0;
}
