#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

//check error and terminate program if it occurs
void isError(int x)
{
    if(x == -1)
    {
        perror("Error");
        exit(0);
    }
}

//check if input is an integer
bool isInt(char *intChar)
{
    int i = 0;
    while (intChar[i])
    {
        if(!isdigit(intChar[i]))
        {
            if(intChar[i]!='-')
            {
                return false;
            }
        }
        i++;
    }
    return true;
}

int main()
{
    bool invalidInput = false;
    bool cont = true;
    int x;

    x = write(STDOUT_FILENO, "Enter a string of integers only: ", 33);
    isError(x);
    while(cont)
    {
        char intStr[100];
        int count = sizeof(intStr);
        int sum = 0;
        char *intChar;

        //Take input from user and replace enter character with null
        int countRead = read(STDIN_FILENO, intStr, count);
        isError(countRead);
        intStr[countRead-1] = NULL;

        //tokenize string and add integers
        intChar = strtok(intStr," ");
        while (intChar != NULL)
        {
            if(isInt(intChar))
            {
                int num;
                sscanf(intChar, "%d", &num);
                sum += num;
                intChar = strtok(NULL, " ");
            }
            else
            {
                invalidInput = true;
                break;
            }
        }

        //if correct input, sum is shown in output else re-prompt for input
        if(!invalidInput)
        {
            char num_char[to_string(sum).size()];
            sprintf(num_char, "%d", sum);
            x = write(STDOUT_FILENO, "The sum is ", 11);
            isError(x);
            x = write(STDOUT_FILENO, num_char, sizeof(num_char));
            isError(x);
            cont = false;
        }
        else
        {
            invalidInput = false;
            x = write(STDOUT_FILENO, "Invalid input! Please re-enter: ", 32);
            isError(x);
        }
    }
    return 0;
}
