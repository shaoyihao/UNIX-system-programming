#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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

int main()
{
	int utmpfd = open(_PATH_UTMP, O_RDONLY); 
	if (utmpfd == -1) 
	{
		perror(_PATH_UTMP);
		exit(-1);
	}

	struct utmp current_record; //utmp文件中每个元素都是一个utmp结构体
	size_t rec_len = sizeof(current_record);
	while ((read(utmpfd, &current_record, rec_len) == rec_len)) //逐个元素（结构体）进行读取
		show_info(&current_record);

	close(utmpfd);
	return 0;
}
