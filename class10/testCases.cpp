#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr
#include <errno.h>

int main()
{
    int x = open("/home/zuha/SP/class10/file.txt", O_RDWR);
    if (x == -1)
    {
        perror("file open: ");
        printf("%d", errno);
    }
    else
    {
        int l = write(x, "Hello", 5);
        if(l = -1){
            perror("write: ");
            printf("%d", errno);
        }
    }


}