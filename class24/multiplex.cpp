#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define TIMEOUT 25
#define BUF_LEN 1024
/* select timeout in seconds */
/* read buffer in bytes */
int main(void)
{
    struct timeval tv;
    fd_set readfds;
    int ret;

    int fd[2];
    ret = pipe(fd);
    if (ret == -1)
    {
        perror("pipe");
    }
    int pid = fork();
    if (pid > 0)
    {
        /* Wait on stdin for input. */
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(fd[0], &readfds);
        /* Wait up to five seconds. */
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;
        /* All right, now block! */
        ret = select(fd[0] + 1,
                     &readfds,
                     NULL,
                     NULL,
                     &tv);
        if (ret == -1)
        {
            perror("select");
            return 1;
        }
        else if (!ret)
        {
            printf("%d seconds elapsed.\n", TIMEOUT);
            return 0;
        }
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char buf[BUF_LEN + 1];
            int len;
            /* guaranteed to not block */
            if (len)
            {
                buf[len] = '\0';
                printf("keyboard read: %s\n", buf);
            }
        }
        if (FD_ISSET(fd[0], &readfds))
        {
            char buf[BUF_LEN + 1];
            int len;
            /* guaranteed to not block */
            len = read(fd[0], buf, BUF_LEN);
            if (len == -1)
            {
                perror("read");
                return 1;
            }
            if (len)
            {
                buf[len] = '\0';
                printf("child read: %s\n", buf);
            }
        }
        return 0;
        fprintf(stderr, "This should not happen!\n");
    }
    else if (pid == 0)
    {
        char buff[50];
        int x = read(STDIN_FILENO, buff, 50);
        x = write(fd[1], buff, strlen(buff));
        if (x == -1)
            perror("write to parent");
    }
    else
        perror("fork");
    return 1;
}