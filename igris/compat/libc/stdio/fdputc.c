#include <stdio.h>
#include <unistd.h>

int fdputc(int c, int fd)
{
    return write(fd, &c, 1);
}
