#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a;
    int b;
    char buffa[7];
    char buffb[7];
    char buffc[7];
    read(STDIN_FILENO, buffa, 7);
    a = atoi(buffa);
    read(STDIN_FILENO, buffb, 7);
    b = atoi(buffb);
    int c = a + b;
    int l = sprintf(buffc,"%d\n",c);
    write(STDOUT_FILENO, buffc, l);
    /*if (l != -1)
    {
        a = atoi(buff);
        
        write(STDOUT_FILENO, a, l);
    }
    else
    {
        perror("read");
    }*/
}