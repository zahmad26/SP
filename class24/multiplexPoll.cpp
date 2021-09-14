#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#define TIMEOUT 5
#define BUF_LEN 1024
/* select timeout in seconds */
/* read buffer in bytes */
int main (void)
{
    int fd[2];
    int x = pipe(fd);
    if(x==-1)
    {
        perror("pipe error");
        return 1;
    }
    int pid = fork();
    if(pid==-1)
    {
        perror("fork error");
        return 1;
    }
    if(pid>0)
    {
        while(1)
        {
            struct pollfd fds[2];
            int ret;

            fds[0].fd = STDIN_FILENO;
            fds[0].events = POLLIN;

            fds[1].fd = fd[0];
            fds[1].events = POLLIN;

            ret = poll(fds, 2, TIMEOUT * 1000);
            if (ret == -1)
            {
                perror ("poll");
                return 1;
            }

            if (!ret)
            {
                printf ("%d seconds elapsed.\n", TIMEOUT);
                return 0;
            }
            if (fds[0].revents & POLLIN){
                char buf[BUF_LEN+1];
                int len;
                /* guaranteed to not block */
                len = read (STDIN_FILENO, buf, BUF_LEN);
                if (len == -1)
                {
                    perror ("read");
                    return 1;
                }
                if (len)
                {
                    buf[len] = '\0';
                    printf ("read: %s\n", buf);
                }
            }
            if (fds[1].revents & POLLIN){
                char buf[BUF_LEN+1];
                int len;
                /* guaranteed to not block */
                len = read (fd[0], buf, BUF_LEN);
                if (len == -1)
                {
                    perror ("read");
                    return 1;
                }
                if (len)
                {
                    buf[len] = '\0';
                    printf ("read from child: %s\n", buf);
                }
            }
        }
        return 0;
    }
    else
    {
        while(1)
        {
            x = write(STDOUT_FILENO, "write message\n", 14);
            if(x==-1)
            {
                perror ("write");
                return 1;
            }

            char buff[100];
            x = read(STDIN_FILENO, buff, 100);
            if(x==-1)
            {
                perror ("read");
                return 1;
            }
            buff[x-1] = NULL;
            x = write(fd[1], buff, 100);
            if(x==-1)
            {
                perror ("write");
                return 1;
            }
        }
    }
}