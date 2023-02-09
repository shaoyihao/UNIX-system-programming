#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 20 
#define ARGLEN 100 

char *makestring(char *buf)
{
    buf[strlen(buf) - 1] = '\0';  
    char *cp = malloc(strlen(buf) + 1); 
    strcpy(cp, buf); 
    return cp;       
}

int main()
{
    char *arglist[MAXARGS + 1]; //存储各参数
  
    char argbuf[ARGLEN]; 
    int numargs = 0;                               
    while (numargs < MAXARGS)
    {
        printf("Please input Arg[%d]: ", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && argbuf[0] != '\n') //读入各参数
            arglist[numargs ++] = makestring(argbuf);
        else if (numargs > 0) //参数读入完毕
        {                            
            arglist[numargs] = NULL; //最后一个参数置为NULL
            execvp(arglist[0], arglist);      
        }
    }

    return 0;
}
