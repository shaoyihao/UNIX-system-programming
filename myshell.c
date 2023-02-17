#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAXLINE 4096 

int main(void)
{
    printf("%% ");

    char buf[MAXLINE];
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';

        pid_t pid = fork();
        if (pid == 0) //子进程
        {
            // execlp(buf, buf, (char *)0);
            char *argv[100]; /* 利用buf的存储空间，划分出一个字符指针数组即可 */

            int i = 0, len, index = 0;
            while (buf[i] == ' ') i ++; /* 处理开头的空格*/
            argv[index ++] = buf + i; /* 或 &buf[i]，第一个非空格的位置 */
            for (len = strlen(buf); i < len; i ++)
            {
                if (buf[i] == ' ') buf[i] = '\0';
                else if ((i - 1 >= 0) && (buf[i - 1] == 0)) argv[index ++] = buf + i; /* 需防止越界，非空格字符的前一个字符为\0，才是参数的起始位置 */
            }
            argv[index] = NULL; /* argv最后一个为NULL */

            execve(argv[0], argv, NULL);
        }

        int status;
        waitpid(pid, &status, 0);
        printf("%% ");
    }

    return 0;
}
