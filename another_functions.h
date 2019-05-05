


// source: https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c
/*
*    This code sends an unsigned int in the socket
*/
static int worker_manager::send_int(unsigned int num, int fd);

// source: https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c

/*
*    This code reads an unsigned int in the socket
*/
static int receive_int(unsigned int *num, int fd);
