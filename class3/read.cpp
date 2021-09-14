#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

int main()
{
    char s[20];
    int l = read(STDIN_FILENO, s, 20);
    if (l != -1)
    {
        write(STDOUT_FILENO, s, l);
    }
    else
    {
        perror("read");
    }
}