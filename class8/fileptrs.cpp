#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd1 = open("/home/zuha/SP/class8/file1.txt",O_RDWR|O_CREAT, S_IRWXU);
    int pid = fork();
    write(fd1,"ABC",3);
    int fd2 = open("/home/zuha/SP/class8/file1.txt",O_APPEND|O_RDWR);
    write(fd2,"XYZ",3);
    if (pid == 0)
    {
        int fd3 = open("/home/zuha/SP/class8/file1.txt",O_RDWR);
        write(fd3, "123", 3);
    }
    else if (pid > 0)
    {
        int fd3 = open("/home/zuha/SP/class8/file1.txt",O_RDWR);   
        write(fd3, "LMN", 3);
    }
    else
        perror("Fork Fail");

}