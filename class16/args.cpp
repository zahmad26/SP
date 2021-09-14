#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argC, char * argV[])
{
    char c[2];
    sprintf(c,"%d\n", argC);
    write(STDOUT_FILENO, c,1);

    for(int i = 0; i<argC;i++){
        
        write(STDOUT_FILENO, argV[i],strlen(argV[i]));
        write(STDOUT_FILENO, " ",1);
    }
    write(STDOUT_FILENO, "\n",1)

}