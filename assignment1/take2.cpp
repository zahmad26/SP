
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <ctype.h>

int checkInput(char input[]){
    int j = 0;
    while (input[j])
    {
        
        if (!isdigit(input[j])){
            return 0;
        } else
        j++;
    }
    return 1;
}

int main()
{
    //instr
    const char *s = "Enter a string containing integers seperated by a space character: \n";
    write(STDOUT_FILENO, s, strlen(s));

    //var
    int sum = 0;
    char totalSum[10];

    //user input
    char buff[128];
    int l = read(STDIN_FILENO, buff, sizeof(buff));
    
    int isValid = 1;
    
    if (l > 0 )
    {
        char *ptr = strtok(buff, " ");
        while (ptr != NULL)
        {
            isValid = checkInput(ptr);
            if(isValid == 0){
                break;
            }
            int i = 0;
            sscanf(ptr, "%d", &i);
            sum += i;
            ptr = strtok(NULL, " ");
        }if(isValid == 1){
            //output
            sprintf(totalSum, "%d\n", sum);
            write(STDOUT_FILENO, "Your sum is: ", 13);
            write(STDOUT_FILENO, totalSum, strlen(totalSum));
        }else{
            write(STDOUT_FILENO, "Input is Not Valid\n", 18);
        }
    }
    else
    {
        write(STDOUT_FILENO, "Input is Not Valid\n", 18);

    }

    return 0;
}