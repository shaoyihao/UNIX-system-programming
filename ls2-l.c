#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void mode_to_str(int mode, char str[])
{
	strcpy(str, "----------");  //长度为1+9的字符串
	//文件类型
	if (S_ISDIR(mode)) str[0] = 'd';
	if (S_ISCHR(mode)) str[0] = 'c';
	if (S_ISBLK(mode)) str[0] = 'b';
	//用户权限
	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';
	//组权限
	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';
	//其它人权限
	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
}

char* uid_to_name(uid_t uid)
{
	struct passwd* u = getpwuid(uid);
	
	if (u == NULL)
	{
		static char num[10];
		sprintf(num, "%d", uid);
		return num;
	}
	else return u->pw_name;
}
char* gid_to_name(gid_t gid)
{
	struct group* g = getgrgid(gid);
	
	if (g == NULL)
	{
		static char num[10];
		sprintf(num, "%d", gid);
		return num;
	}
	else return g->gr_name;
}

void show_stat(char filename[])
{
    struct stat info;
    stat(filename, &info);
    
    char modestr[11];
    mode_to_str(info.st_mode, modestr);
    printf("%s %4d %-8s %-8s %8ld %.12s %s\n", modestr, (int)info.st_nlink, uid_to_name(info.st_uid), gid_to_name(info.st_gid), (long)info.st_size, ctime(&info.st_mtime), filename);
}

void show(char path[])
{
    DIR* dir_ptr = opendir(path); //获取该目录的指针

    if (dir_ptr == NULL) fprintf(stderr, "ls1: can't open %s\n", path);
    else
    {
        struct dirent* rec; //用于指向各条记录
        while ((rec = readdir(dir_ptr)) != NULL)
            show_stat(rec->d_name); //输出普通文件或子目录的名称

        closedir(dir_ptr);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) show(".");
    else
    {
        int i;
        for (i = 1; i < argc; i ++)
        {
            printf("%s:\n", argv[i]);
            show(argv[i]);
        }
    }
    return 0;
}
