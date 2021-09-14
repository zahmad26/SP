#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

int main()
{
    write(-3, "Hello World",11);
    if(errno == EBADF){
        perror("EBADF");
    }
    printf("%d\n",errno);
}