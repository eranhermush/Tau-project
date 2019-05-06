
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;


class another_functions{


public:

	// source: https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c
	/*
	*    This code sends an unsigned int in the socket
	*/
	static int send_int(unsigned int num, int fd);

	// source: https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c

	/*
	*    This code reads an unsigned int in the socket
	*/
	static int receive_int(unsigned int *num, int fd);
};