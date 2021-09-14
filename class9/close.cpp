#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr
#include <string.h>

int main()
{
    int x = open("/home/zuha/SP/class9/file.txt", O_RDWR, S_IRWXU);
    if (x == -1)
    {
        perror("file open: ");
    }
    else
    {
        char buff[50];
        int l = read(STDIN_FILENO, buff, 50);
        write(x, buff, l); 
        getchar();
    }
}