#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr

int main()
{
    int x = open("/home/zuha/SP/class6/orignal.txt", O_RDONLY);
    if (x == -1)
    {
        perror("x file open: ");
    }
    int y = open("/home/zuha/SP/class6/newcopy.txt", O_CREAT|O_RDWR, S_IRWXU);
    if (y == -1)
    {
        perror("y file open: ");
    }
    else
    {
        char buff[50];
        int l = read(x, buff, 50);
        while (l != 0)
        {
            int r = write(y, buff, l);
            if(r <= 0){
                write(STDOUT_FILENO, "could not write", 15);
            }
            l = read(x, buff, 50);   
        }
    }
}