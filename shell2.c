#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 20 
#define ARGLEN 100 

char* makestring(char *buf)
{
    buf[strlen(buf) - 1] = '\0';  
    char *cp = malloc(strlen(buf) + 1); 
    strcpy(cp, buf); 
    return cp;       
}

int main()
{
    char *arglist[MAXARGS + 1];
    char argbuf[ARGLEN];       

    int numargs = 0;
    while (numargs < MAXARGS)
    {
        printf("Please input Arg[%d]: ", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && argbuf[0] != '\n')
            arglist[numargs ++] = makestring(argbuf);
        else if (numargs > 0) //参数输入完毕
        {                            
            arglist[numargs] = NULL;

            int st, pid = fork(); 
            if (pid == 0) execvp(arglist[0], arglist); //在子进程中执行该程序
            else 
            {
                wait(&st); //此处每次只会运行一个子进程，因此等待该子进程结束即可
                printf("child process exited with status %d,%d\n", st >> 8, st & 0377);
            }

            numargs = 0;  //准备接收新的命令        
        }
    }

    return 0;
}
