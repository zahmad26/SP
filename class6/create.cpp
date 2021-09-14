#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr

int main()
{
    int x = open("/home/zuha/SP/class6/file2.txt", O_CREAT|O_RDWR);
    if (x == -1)
    {
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