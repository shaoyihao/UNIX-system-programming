#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define NULLUT ((struct utmp*)NULL)

#define CNT 16  //缓冲区的容量
#define UTSIZE (sizeof(struct utmp)) //一个utmp结构体的大小

char utmpbuf[CNT * UTSIZE]; //用一个能容纳16个utmp结构体的数组作为缓冲区
int recNum; //记录缓冲区中的记录数目
int curRec; //记录当前所指向的缓冲区中的记录
int fd_utmp = -1; //全局变量

void utmp_open()
{
    fd_utmp = open(_PATH_UTMP, O_RDONLY);
    curRec = recNum = 0;

    if (fd_utmp == -1) 
	{
		perror(_PATH_UTMP);
		exit(-1);
	}
}
void utmp_close()
{
    if (fd_utmp != -1) close(fd_utmp);
}
int utmp_load()  //从文件中加载一部分数据（若干条记录）到缓冲区中  （更新缓冲区）
{
    ssize_t byteNum = read(fd_utmp, utmpbuf, CNT * UTSIZE);
    recNum = byteNum / UTSIZE;
    curRec = 0; //初始化指针
    return recNum;
}
struct utmp* read_a_rec()   //从缓冲区返回一条记录(的指针)
{
    if (fd_utmp == -1) return NULLUT; 

    /*当缓冲区中的所有记录都已被读取后，再取新的一组数据到缓冲区中*/
    if (curRec == recNum && utmp_load() == 0) return NULLUT; //用到了“短路”
    
    /*缓冲区中的数据还没有处理完时*/
    struct utmp* rec = (struct utmp*)&utmpbuf[curRec * UTSIZE]; //从缓冲区中读取一条记录
    curRec ++; //指向下一条记录
    return rec;
}

void showtime(long t)
{
	char* str = ctime(&t);   //将秒数转化为一个日期字符串
	printf("%12.12s", str);
	// printf("%12.12s", str + 4);  从月份开始输出
}
void show_info(struct utmp* utbufp)
{
	if (utbufp->ut_type != USER_PROCESS) return;
	printf("%-8.8s %-8.8s %10ld (%-8.8s) ", utbufp->ut_name, utbufp->ut_line, utbufp->ut_time, utbufp->ut_host);
	showtime(utbufp->ut_time);
	puts("");
}

/*修改后的主函数没有直接对open、read、close进行调用，而是调用具有缓冲模式的函数接口。*/
int main()
{
    utmp_open();

	struct utmp *r; 
	while ((r = read_a_rec()) != NULLUT) 
		show_info(r);

	utmp_close();
	return 0;
}
