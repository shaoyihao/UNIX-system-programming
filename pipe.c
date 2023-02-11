#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int pipefd[2];        
    pipe(pipefd);

    int pid = fork(); //父进程与子进程均连接着该管道

    if (pid) 
    {                      
        close(pipefd[1]);   //父进程不向管道发送
        dup2(pipefd[0], 0); //父进程从管道中进行读入
        close(pipefd[0]); 
        execlp(argv[2], argv[2], NULL);
    }

    close(pipefd[0]);   //子进程不从管道中读入
    dup2(pipefd[1], 1); //子进程将数据输出到管道
    close(pipefd[1]); 
    execlp(argv[1], argv[1], NULL);
}

/*
用法：./pipe cmd1 cmd2  效果等价与 cmd1 | cmd2
*/
