#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <list>
#include <fcntl.h>
#include <signal.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <ctime>
#include <pthread.h>

int main()
{
    int pid = fork();
    if(pid == -1){
        perror("fork");
        exit(1);
    }
    if (pid > 0)
    {
        int x, status;
        x = waitpid(pid, &status, 0);
        if (x == -1)
        {
            perror("waitpid failed");
            exit(1);
        }
        if (WIFEXITED(status))
        {
            int sum = WEXITSTATUS(status);
            char buff[5];
            sprintf(buff, "The sum is: %d\n", sum);
            x = write(STDOUT_FILENO ,buff, strlen(buff));
            if(x == -1){
                perror("write");
                exit(1);
            }
        }
    }
    else if (pid == 0)
    {

        int x;
        char buff[10], buff1[10];

        x = read(STDIN_FILENO, buff, 10);
        if (x == -1)
        {
            perror("read 1");
            exit(1);
        }
        x = read(STDIN_FILENO, buff1, 10);
        if (x == -1)
        {
            perror("read 2");
            exit(1);
        }

        int num1;
        int num2;

        x = sscanf(buff, "%d", &num1);
        if (x == -1)
        {
            perror("convert num");
            exit(1);
        }
        x = sscanf(buff1, "%d", &num2);
        if (x == -1)
        {
            perror("convert num");
            exit(1);
        }

        int sum = num1 + num2;
        exit(sum);
    }
}