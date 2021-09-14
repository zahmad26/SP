#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int main()
{
    int fd = open("/home/zuha/SP/class20/file.txt", O_RDWR, S_IRWXU);
    int fork_status = fork();
    if (fork_status > 0)
    {
        write(STDOUT_FILENO, "p", 1);
        int check = write(fd, "12345", 5);
        if (check == -1)
            perror("write to file");
        // char buff[5];
        // sprintf(buff, "%d", fork_status);
        // write(STDOUT_FILENO, buff, 5);
        
        int kill_status = kill(fork_status, SIGUSR1);
        if (kill_status == -1)
            perror("signal not sent");
        // pause();
        // int check = write(fd,"12345", 5);
    }
    else if (fork_status == 0)
    {
        write(STDOUT_FILENO, "c", 1);
        // for (;;)
        //     pause();
        
        char buff[10] = {};
        int check = read(fd, buff, 10);
        write(STDOUT_FILENO, "s", 1);
        if (check == -1)
            perror("read from file");
        check = write(STDOUT_FILENO, buff, check);
        if (check == -1)
            perror("write output");
        // kill(getppid(),SIGUSR1);
    }
    else
    {
        perror("fork fail");
    }
}