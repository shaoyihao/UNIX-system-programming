#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096   //设置缓冲区的大小
#define COPYMODE 0644
char buf[BUFFERSIZE];

void err(char* str1, char* str2)
{
    fprintf(stderr, "ERROR: %s", str1);
    perror(str2);
    exit(1);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s source destination\n", argv[0]);
        exit(1);
    }

    int input_fd = open(argv[1], O_RDONLY), output_fd = creat(argv[2], COPYMODE);
    if (input_fd == -1) err("Can't open ", argv[1]);
    if (output_fd == -1) err("Can't open ", argv[2]);

    int len;
    while ((len = read(input_fd, buf, BUFFERSIZE)) > 0) 
        if (write(output_fd, buf, len) != len) err("Write error: ", argv[2]); //读出的内容直接写入到文件2中
    if (len == -1) err("Read error: ", argv[1]);

    if (close(input_fd) == -1 || close(output_fd) == -1) err("Closing Error", "");
    return 0;
}
