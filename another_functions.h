
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

/*
 we dint use this class
*/
class another_functions{


public:

	// source: https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c
	/*
	*    This code sends an unsigned int in the socket
	*/
	static int send_int(unsigned int num, int fd) ;

	// source: https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c

	/*
	*    This code reads an unsigned int in the socket
	*/
	static int receive_int(unsigned int *num, int fd);

	/* 
	*	Sends a message of length len via the socket fd
		Returns 0 on success and -1 on failure
	*/
	static int send_chars(int fd, const char* message, int len);

	/* 
	*	Read a message of length len from the socket fd
		Returns 0 on success and -1 on failure
	*/
	static int recv_chars(int fd, const char* message, int len);
};