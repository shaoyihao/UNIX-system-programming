#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef void Myfunc(const char *, const struct stat *, int);
//将返回值为 int 型，参数为 char*、struct stat*、int 的函数称为 Myfunc 类型

#define FTW_F 1	  //非目录文件
#define FTW_D 2	  //目录文件
#define FTW_DNR 3 //读取失败的目录
#define FTW_NS 4  //stat失败的文件

long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot, n4096;
char name[1024]; //路径

int filesize;
char *filebuf, *cmpfilebuf;
int res = 0; //内容相同的文件数

int cnt; //列举出的名字数目
char *str[100]; //存储所列举出的名字（只保留文件名）
int res2 = 0; //名字相同的文件数

// void getAbsolutePath(const char *pathname, char *absolutePath)
// {
//     char *tmp = (char*)malloc(1024);
//     getcwd(tmp, 1024);
//
//     chdir(pathname);
//     getcwd(absolutePath, 1024);
//
//     char *p = absolutePath + strlen(absolutePath);
//     if (*(p - 1) != '/')  /* 保证目录的绝对路径以斜杠结尾 */
//     {      
//         *p++ = '/';
//         *p = '\0';
//     }
//
//     chdir(tmp);
// }
int getFileNamePos(const char *pathname) //获取文件名在路径中的位置
{
    int i, pos = 0;
    for (i = strlen(pathname) - 1; i >= 0; i --) 
    {
        if (pathname[i] == '/') 
        {
            pos = i;
            break;
        }
    }
    return (pos == 0) ? pos : pos + 1; //有可能不包含 '/' 
}

void count(const char *pathname, const struct stat *statptr, int type) //将文件加入统计
{
    switch (type)
	{
	case FTW_F:
        // printf("非目录文件: %s\n", pathname);
        if (statptr->st_size > 4096) n4096 ++; //统计长度大于 4096B 的文件数目
		switch (statptr->st_mode & S_IFMT)
		{
		case S_IFREG: nreg++; break;
		case S_IFBLK: nblk++; break;
		case S_IFCHR: nchr++; break;
		case S_IFIFO: nfifo++; break;
		case S_IFLNK: nslink++; break;
		case S_IFSOCK: nsock++; break;
		}
		break;
	case FTW_D: 
        // printf("目录文件: %s\n", pathname);
		ndir++; break;
	case FTW_DNR:
		printf("can't read directory %s", pathname);
		break;
	case FTW_NS:
		printf("stat error for %s", pathname);
		break;
	default:
		printf("unknown type %d for pathname %s", type, pathname);
	}
}
void cmp(const char *pathname, const struct stat *statptr, int type) //判断该文件与所给文件的内容是否相同
{
    if (type == FTW_F && (statptr->st_mode & S_IFMT) == S_IFREG && statptr->st_size == filesize)
    {
        int fd = open(pathname, O_RDONLY);
        
        read(fd, cmpfilebuf, filesize);
        close(fd);

        if (strcmp(filebuf, cmpfilebuf) == 0)
        {
            res ++;
            puts(pathname);
        }
    }
}
void cmpname(const char *pathname, const struct stat *statptr, int type) //判断该文件的文件名是否出现在所给的列表中
{
    if (type == FTW_F && (statptr->st_mode & S_IFMT) == S_IFREG)
    {
        int i = 0;
        for (i = 0; i < cnt; i ++)
        {
            if (strcmp(pathname + getFileNamePos(pathname), str[i]) == 0)
            {
                res2 ++;
                puts(pathname);
                break;
            }
        }
    }
}

void dopath(Myfunc *f) //处理当前name所指的文件或目录
{
	struct stat statbuf;
    
	if (lstat(name, &statbuf) < 0) //无法stat的文件
	{
		f(name, &statbuf, FTW_NS);  
		return;
	}
	if (S_ISDIR(statbuf.st_mode) == 0) //非目录文件
	{
		f(name, &statbuf, FTW_F);  
		return;
	}
	f(name, &statbuf, FTW_D);   //目录文件
    
	//下面对该目录文件进行遍历
	int n = strlen(name);
	name[n] = '/', name[n + 1] = '\0';

	DIR *dp = opendir(name);
	if (dp == NULL) f(name, &statbuf, FTW_DNR); //无法打开的目录

	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL)
	{
		if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) continue;
		strcpy(&name[n + 1], dirp->d_name);
		dopath(f); //递归对该子文件进行处理
	}
	name[n] = '\0'; //恢复现场

	closedir(dp);
}

int main(int argc, char* argv[])
{
    if (!(argc == 2 || (argc == 4 && strcmp(argv[2], "-comp") == 0) || (argc >= 4 && strcmp(argv[2], "-name") == 0)))
    {
        puts("usage:  myfind <pathname> [-comp <filename> | -name <str>…]");
        return 0;
    }

    struct stat statbuf;
    lstat(argv[1], &statbuf);

    if (S_ISDIR(statbuf.st_mode) == 0) strcpy(argv[1], "."); //若所给的pathname是一个文件，则遍历当前所在的目录
    strcpy(name, argv[1]);

    if (argc == 2)
    {
	    dopath(count);

	    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
	    if (ntot == 0) ntot = 1;
	    printf("regular files  = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
	    printf("directories    = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
	    printf("block special  = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
	    printf("char special   = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
	    printf("FIFOs          = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
	    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
	    printf("sockets        = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    }
    else if (argc == 4 && strcmp(argv[2], "-comp") == 0)
    {
        if ((lstat(argv[3], &statbuf) < 0))
        {
            printf("lstat失败: %s\n", argv[3]);
            return 0;
        } 
        if (S_ISDIR(statbuf.st_mode))
        {
            printf("该文件是一个目录: %s\n", argv[3]);
            return 0;
        }

        filesize = statbuf.st_size;
        int fd = open(argv[3], O_RDONLY);
        filebuf = (char*)malloc(sizeof(char) * filesize);
        cmpfilebuf = (char*)malloc(sizeof(char) * filesize);
        read(fd, filebuf, filesize);
        close(fd);

        dopath(cmp);

        printf("与 %s 内容相同的文件共 %d 个。", argv[3], res);
    }
    else if (argc >= 4 && strcmp(argv[2], "-name") == 0)
    {
        cnt = argc - 3;

        int i;
        for (i = 0; i < cnt; i ++)
        {
            str[i] = (char*)malloc(1024);
            strcpy(str[i], argv[3 + i] + getFileNamePos(argv[3 + i]));
        }

        dopath(cmpname);

        printf("名字相同的文件共 %d 个。", res2);
    }

    return 0;
}
