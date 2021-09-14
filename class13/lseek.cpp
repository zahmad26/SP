#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    int fd = open("/home/zuha/SP/class13/file.txt", O_RDWR, S_IRWXU);
   //write(fd, "Hello",5);
    int pos = lseek(fd, 0, SEEK_CUR);
    if(pos==-1){
        perror("lseek ");
    }
    printf("%d", pos);

   char buff[1];
   read(fd,buff,1);
   pos = lseek(fd, 0, SEEK_CUR);
   printf("\n%d\n", pos);
     // write(fd, "buff", 4);

    printf(buff);
    
}