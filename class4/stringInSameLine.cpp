#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

int main()
{
    char s1[20];
    char s2[20];
    int l1 = read(STDIN_FILENO, s1, 20);
    int l2 = read(STDIN_FILENO, s2, 20);
    s1[l1-1] = ' ';
    if (l1 > 0 && l2 > 0 )
    {
        write(STDOUT_FILENO, s1, l1);
        write(STDOUT_FILENO, s2, l2);
    }
    else
    {
        perror("read");
    }
}