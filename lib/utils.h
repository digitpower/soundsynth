#ifndef UTILS_H
#define UTILS_H

#include "audioengineparams.h"

#include <sys/types.h> //for ssize_t

ssize_t readBytesFromSocket(int fd, char *ptr, size_t n);
short strToShort(char* str, int length);
#endif
