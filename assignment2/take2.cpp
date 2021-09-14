#include <unistd.h>
#include <string.h>
#include <stdio.h>  //printf
#include <iostream> //isdigit

void checkError(int rw, char *y)
{
    if (rw == -1)
    {
        perror(y);
        exit(0);
    }
}
bool checkInt(char *input)
{
    int i = 0;
    while (input[i])
    {
        if (!isdigit(input[i]))
        {
            return false;
        }
        i++;
    }
    return true;
}
int main()
{

    //instr
    const char *s = "Enter a string containing integers seperated by a space character and end list with ' ;': \n";
    int w = write(STDOUT_FILENO, s, strlen(s));
    checkError(w, "Instruction write");

    //var
    int sum = 0;
    char totalSum[10];
    bool exit = false;
    bool validInput = true;

    while (!exit)
    {

        //user input
        char buff[128];
        int r = read(STDIN_FILENO, buff, sizeof(buff));
        checkError(r, "user input read");
        buff[r - 1] = NULL;

        //tokenize
        char *ptr = strtok(buff, " ");
        char x;
        while (ptr != NULL)
        {
            x = *ptr;
            if (x == ';')
            {
                //output
                sprintf(totalSum, "%d\n", sum);
                w = write(STDOUT_FILENO, "Your sum is: ", 13);
                w = write(STDOUT_FILENO, totalSum, strlen(totalSum));
                checkError(w, "output write");
                ptr = strtok(NULL, " ");
                sum = 0;
            }
            else if (checkInt(ptr))
            {
                validInput = true;
                int i;
                sscanf(ptr, "%d", &i);
                sum += i;
                ptr = strtok(NULL, " ");
            }
            else
            {
                validInput = false;
                break;
            }
        }
        if (!validInput)
        {
            sum = 0;
            write(STDOUT_FILENO, "Input is Not Valid, Please Re-enter\n", 37);
            checkError(w, "invalid input write");
        }
    }
}