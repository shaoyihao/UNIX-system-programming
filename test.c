#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSIZE 10010

char buf[BUFSIZE];

int main()
{
    int fd = open(".", O_RDONLY);
    if (fd == -1) perror("ERRROR");
    
    read(fd, buf, BUFSIZE);

    puts(buf);
    return 0;
}