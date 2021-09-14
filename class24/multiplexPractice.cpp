#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#define TIMEOUT 10
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
            struct timeval tv;
            fd_set readfds;
            int ret;
            /* Wait on stdin for input. */
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            FD_SET(fd[0], &readfds);
            /* Wait up to five seconds. */
            tv.tv_sec = TIMEOUT;
            tv.tv_usec = 0;
            /* All right, now block! */
            ret = select (fd[0] + 1,
                          &readfds,
                          NULL,
                          NULL,
                          &tv);
            if (ret == -1)
            {
                perror ("select");
                return 1;
            }
            else if (!ret)
            {
                printf ("%d seconds elapsed.\n", TIMEOUT);
                return 0;
            }
            /*
            * Is our file descriptor ready to read?
            * (It must be, as it was the only fd that
            * we provided and the call returned
            * nonzero, but we will humor ourselves.)
            */
            if (FD_ISSET(STDIN_FILENO, &readfds))
            {
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
            if (FD_ISSET(fd[0], &readfds))
            {
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
        fprintf (stderr, "This should not happen!\n");
        return 1;
    }
    else
    {
//        x = write(STDOUT_FILENO, "write message\n", 14);
//        if(x==-1){
//            perror ("write");
//            return 1;
//        }
        while(1)
        {
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