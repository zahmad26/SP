#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr

int main()
{
    int x = open("/home/zuha/SP/class5/file.txt", O_RDONLY);
    if (x == -1)
    {
        perror("file open: ");
    }
    else
    {
        char buff[50];
        int l = read(x, buff, 50);
        while (l != 0)
        {
            write(STDIN_FILENO, buff, l);
            l = read(x, buff, 50);   
        }
    }
}