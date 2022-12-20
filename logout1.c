#include <unistd.h>
#include <fcntl.h>
#include <utmp.h>
#include <string.h>
#include <time.h>

#define UTSIZE (sizeof(struct utmp))

void logout_tty(char* line)
{
    int fd = open(_PATH_UTMP, O_RDONLY);
    
    struct utmp rec;
    while (read(fd, &rec, UTSIZE) == UTSIZE) //枚举每条记录，查找指定终端名
        if (strncmp(rec.ut_line, line, sizeof(rec.ut_line) == 0))
        {
            rec.ut_type = DEAD_PROCESS;
            time(&rec.ut_time); //修改属性

            lseek(fd, -UTSIZE, SEEK_CUR); //调整指针到本条记录的首部
            write(fd, &rec, UTSIZE); //重写本记录
        }

    close(fd);
    return;
}