#include "another_functions.h"


int another_functions::send_int(unsigned int num, int fd)
{
    uint32_t conv = htonl(num);
    char *data = (char*)&conv;
    unsigned int left = sizeof(conv);
    unsigned int rc = 0;
    do {
        rc = write(fd, data, left);
        if (rc < 0) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // use select() or epoll() to wait for the socket to be writable again
            }
            else if (errno != EINTR) {
                perror( "Error in write in send_int: \n" );
                return -1;
            }
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);
    return 0;
} 

int another_functions::receive_int(unsigned int *num, int fd)
{
    uint32_t ret;
    char *data = (char*)&ret;
    unsigned int left = sizeof(ret);
    unsigned int rc = 0;
    do {
        rc = read(fd, data, left);
        if (rc <= 0) { /* instead of ret */
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // use select() or epoll() to wait for the socket to be readable again
            }
            else if (errno != EINTR) {
                return -1;
            }
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);
    *num = ntohl(ret);

    return 0;
}

int another_functions::send_chars(int fd, const char* message, int len){
    int ret = 0, sent = 0;
    while(sent < len){
        ret = send(fd, message + sent, len - sent, 0);
        if(ret <= 0){
            /* error or could not send any bytes*/
            return -1;
        }
        sent += ret;
    }
    return 0;
}

int another_functions::recv_chars(int fd, const char* message, int len){
    int ret = 0, sent = 0;
    while(sent < len){
        ret = send(fd, message + sent, len - sent, 0);
        if(ret <= 0){
            /* error or could not send any bytes*/
            return -1;
        }
        sent += ret;
    }
    return 0;
}
