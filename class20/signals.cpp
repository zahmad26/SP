#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void sigint_handler(int signo)
{
    int x;
    char ans[2];
    x = write(STDOUT_FILENO, "Would you like to exit? y/n\n", 29);
    if (x == -1)
    {
        perror("write");
        exit(1);
    }
    x = read(STDIN_FILENO, ans, 1);
    if (x == -1)
    {
        perror("read");
        exit(1);
    }
    if(strcmp(ans, "y") == 0){
        exit(0);
    }
}

int main(void)
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        write(STDOUT_FILENO, "Cannot handle SIGINT\n", 22);
        exit(EXIT_FAILURE);
    }
    return (0);
}