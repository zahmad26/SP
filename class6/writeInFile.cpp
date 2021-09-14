#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr

int main()
{
    int x = open("/home/zuha/SP/class5/file1.txt", O_RDWR);
    if (x == -1)
    {
        perror("file open: ");
    }
    else
    {
        char y = 'y';
        int l = write(x, &y, 2);
        if (l <= 0)
        {
            write(STDOUT_FILENO, "no value printed", 17);
        }
    }
}