#include <stdio.h>
#include <sys/stat.h>

void show(char* name, struct stat info)
{
    printf("mode: %o\n", info.st_mode);
    printf("links: %d\n", info.st_nlink);
    printf("user: %d\n", info.st_uid);
    printf("group: %d\n", info.st_gid);
    printf("size: %d\n", info.st_size);
    printf("modtime: %d\n", info.st_mtime);
    printf("name: %s\n", name);
}

int main(int argc, char* argv[])
{
    struct stat info;

    if (argc == 2 && (stat(argv[1], &info) != -1)) show(argv[1], info);
    else perror(argv[1]);

    return 0;
}