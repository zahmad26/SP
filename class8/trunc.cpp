#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd1 = open("/home/zuha/SP/class8/file3.txt",O_RDWR|O_CREAT, S_IRWXU);
    if(fd1 == -1) perror("open");
    int x = write(fd1,"123456",6);
    printf("%d",x);
    if(x == -1) perror("write");
    int fd2 = open("/home/zuha/SP/class8/file3.txt",O_TRUNC|O_RDONLY);
    if(fd1 == -1) perror("open");
    // write(fd2,"A",1);
    // write(fd1,"910",3);
}