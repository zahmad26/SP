#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/unistd.h>
int main(int argC, char *argV[])
{
    if (argC > 1)
    {
        int pid = atoi(argV[0]);
        kill(pid, 15);
    } 
    char c[2];
    sprintf(c, "%d\n", argC);
    write(STDOUT_FILENO, c, 2);
}