#include <errno.h>   //for errno
#include <unistd.h>  //for read
#include "utils.h"

ssize_t readBytesFromSocket(int fd, char *ptr, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) <= 0) {
            if (nread < 0 && (errno == EINTR || errno == EAGAIN))
                nread = 0;
            /* and call write() again */
            else
                return (-1);/* error */
        }
        nleft -= nread;
        ptr += nread;
    }
    return (n);
}

short strToShort(char* str, int length)
{
    short res = 0;
    for (int i = 0;i < length;i++)
        res = res*10 + str[i]-'0';
    return res;
}

/*
** unpacki16() -- unpack a 16-bit int from a char buffer (like ntohs())
*/
int unpacki16(unsigned char *buf)
{
    unsigned int i2 = ((unsigned int)buf[0]<<8) | buf[1];
    int i;

    // change unsigned numbers to signed
    if (i2 <= 0x7fffu) { i = i2; }
    else { i = -1 - (unsigned int)(0xffffu - i2); }

    return i;
}
