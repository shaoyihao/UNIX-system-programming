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
    while (read(fd, &rec, UTSIZE) == UTSIZE)
        if (strncmp(rec.ut_line, line, sizeof(rec.ut_line) == 0))
        {
            rec.ut_type = DEAD_PROCESS;
            time(&rec.ut_time);
            lseek(fd, -UTSIZE, SEEK_CUR); //调整指针位置
            write(fd, &rec, UTSIZE);
            // close(fd);
            // return;
        }

    close(fd);
    return;
}