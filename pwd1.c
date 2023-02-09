#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

ino_t get_inode(char *fname) //获取文件名对应的 i 结点号
{
    struct stat info;
    stat(fname, &info);    
    return info.st_ino;
}

void inum_to_name(ino_t inode, char *namebuf, int buflen) //获取当前目录下某i结点号对应的名字，存入namebuf中
{
    DIR *dir_ptr = opendir("."); //读取目录文件

    struct dirent *p;
    while ((p = readdir(dir_ptr)) != NULL)  //遍历其中的项，查找i结点号
        if (p->d_ino == inode)
        {
            strncpy(namebuf, p->d_name, buflen); //将 p->name 存入 namebuf 中
            namebuf[buflen - 1] = '\0';  //strncpy不会在复制的内容后面自动加'\0'，这里为避免名字长度超出数组，将最后一个位置置为\0 （just in case）

            closedir(dir_ptr);
            return;
        }
}

void printpath(ino_t this_inode) //打印从根结点到本结点n的路径
{
    if (get_inode("..") == this_inode) return; //若当前结点已是根结点，则递归终止

    chdir(".."); //切换到上层结点

    char name[BUFSIZ];
    inum_to_name(this_inode, name, BUFSIZ); //获取结点n在目录中的名字

    printpath(get_inode(".")); //顺序很重要（每一次递归都会导致所处目录的改变）
    printf("/%s", name); //先递归（输出前面的路径）后打印（当前结点的名字）
}

int main()
{
    printpath(get_inode(".")); 
    putchar('\n');
    return 0;
}
