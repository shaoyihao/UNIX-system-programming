#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main()
{
    int fd = open("XXX", O_RDONLY);
    if (fd == -1) perror("Can't open file");
    return 0;
}