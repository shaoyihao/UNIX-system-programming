#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

int print_X_line()  //判断是否需要继续输出，返回需要再输出的行数
{
    printf("\033[7m more? \033[m");

    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == 'q') return 0;       //再输出0行（即退出）
        if (c == ' ') return PAGELEN; //再输出PAGELEN行（即输出一屏）
        if (c == '\n') return 1;      //再输出一行
    }
    return 0;
}

void display(FILE* fp)  //输出fp中的内容
{
    int num_of_line = 0; //当前显示的行数（主要考虑 num_of_line 与 PAGELEN 的差值）

    char line[LINELEN];
    while (fgets(line, LINELEN, fp))  //从fp中读取数据存入line中
    {
        if (num_of_line == PAGELEN) // 显示一屏后
        {
            int t = print_X_line();

            if (t == 0) break;
            else num_of_line -= t;
        }
        if (fputs(line, stdout) == EOF) exit(1); //将line中的数据输出到stdout中
        num_of_line ++;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1) display(stdin);
    else 
    {
        FILE* fp;
        for (int i = 1; i < argc; i ++)  //枚举命令行给出的各个文件名
        {
            if ((fp = fopen(argv[i], "r")) == NULL) exit(1);
            display(fp);
            fclose(fp);
        }
    }
    return 0;
}