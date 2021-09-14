#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr
#include <errno.h>
#include <error.h>

int main()
{
    int x = open("/home/zuha/SP/class7/file.txt", O_RDWR);
    if (x == -1)
    {
        printf("%d",EBADF);
        perror("file open: ");
    }
    else
    {
        int l = write(x, "Hello", 5);
        if (l <= 0)
        {
            write(STDOUT_FILENO, "no value printed", 17);
        }
    }
}