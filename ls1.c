#include <stdio.h>
#include <dirent.h>

void ls(char path[])
{
    DIR* dir_ptr = opendir(path); //获取该目录的指针

    if (dir_ptr == NULL) fprintf(stderr, "ls1: can't open %s\n", path);
    else
    {
        struct dirent* rec; //指向各条记录
        while ((rec = readdir(dir_ptr)) != NULL)
            puts(rec->d_name); //输出普通文件或子目录的名称
            
        closedir(dir_ptr);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) ls("."); //没有附加参数
    else 
        for (int i = 1; i < argc; i ++)
        {
            printf("%s:\n", argv[i]);
            ls(argv[i]);
        }

    return 0;
}