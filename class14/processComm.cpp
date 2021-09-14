#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void chkErr(int r, const char *message)
{
    if (r == -1)
    {
        perror(message);
        exit(0);
    }
}

int main()
{
    int fd1[2];
    int fd2[2];

    int ret = pipe(fd1);
    chkErr(ret, "create pipe 1");
    ret = pipe(fd2);
    chkErr(ret, "create pipe 2");

    int pid = fork();
    chkErr(pid, "fork");
    if (pid > 0)
    {
        int x;

        char buff[10], buff1[10], sum[10];
        close(fd1[0]);
        close(fd2[1]);
        //take input
        x = read(STDIN_FILENO, buff, 10);
        chkErr(x, "read input 1");
        x = read(STDIN_FILENO, buff1, 10);
        chkErr(x, "read input 2");

        //write to server
        x = write(fd1[1], buff, strlen(buff));
        chkErr(x, "write input 1 to server");
        x = write(fd1[1], buff1, strlen(buff1));
        chkErr(x, "write input 2 to server");

        //read from server
        x = read(fd2[0], sum, 2);
        chkErr(x, "read sum from server");

        //output
        x = write(STDOUT_FILENO, sum, strlen(sum));
        chkErr(x, "write output");
    }
    else if (pid == 0)
    {

        int num1;
        int num2;
        int sum = 0 ;
        char buffsum[100];
        char buff3[100];
        char buff4[100];
        close(fd1[1]);
        close(fd2[0]);

        //read from client
        int x = read(fd1[0], buff3,2);
        chkErr(x,"read input from client");
        x = read(fd1[0],buff4,3);
        chkErr(x, "read input 2 from client");


        int check = sscanf(buff3,"%d",&num1);
        chkErr(check,"convert to num1");
        check = sscanf(buff4,"%d",&num2);
        chkErr(check,"convert to num2");

        sum = num1 + num2;

        check = sprintf(buffsum,"%d",sum);
        chkErr(check, "convert sum");

        //write to client
        write(fd2[1],buffsum,check);


    }
    return 0;
}