#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void chkErr(int r, const char *message)
{
    if (r == -1)
    {
        perror(message);
        exit(0);
    }
}
int checkInput(char *input)
{
    int j = 0;
    while (input[j])
    {

        if (isalpha(input[j]))
        {
            return 0;
        }
        j++;
    }
    return 1;
}
int checkDiv(char *input)
{
    int j = 0;
    while (input[j])
    {

        if (input[j] == '0')
        {
            return 0;
        }
        j++;
    }
    return 1;
}

int main()
{
    int fd1[2];
    int fd2[2];

    int ret = pipe(fd1);
    chkErr(ret, "create pipe 1");
    ret = pipe(fd2);
    chkErr(ret, "create pipe 2");

    int pid = fork();
    chkErr(pid, "fork");
    if (pid > 0)
    {
        close(fd1[0]);
        close(fd2[1]);
        write(STDOUT_FILENO, "Enter commands on the terminal.\nPossible commands:\n****add <list<int>>\n****sub <list<int>>\n****mul <list<int>>\n****div <list<int>>\n****run <path>\n****exit\n", 156);
        while (true)
        {
            int x;
            char command[100] = {};
            char output[100] = {};
            //take input
            x = write(STDOUT_FILENO, ">", 1);
            chkErr(x, "write terminal");
            x = read(STDIN_FILENO, command, 100);
            chkErr(x, "read command");
            
            //write to server
            x = write(fd1[1], command, strlen(command));
            chkErr(x, "write command to server");

            //read output from server
            x = read(fd2[0], output, 100);
            chkErr(x, "read output from server");
            if (strcmp(output, "exit") == 0)
            {
                exit(0);
            }
            else
            {
                //output
                x = write(STDOUT_FILENO, output, x);
                chkErr(x, "write output");
            }
        }
    }
    else if (pid == 0)
    {

        close(fd1[1]);
        close(fd2[0]);
        while (true)
        {
            char buff_comm[100] = {};
            char output[100] = {};
            char buff4[100] = {};
            //read from client
            int x = read(fd1[0], buff_comm, 100);
            chkErr(x, "read command from client");

            buff_comm[x - 1] = NULL;

            if (strcmp(buff_comm, "exit") == 0)
            {
                write(fd2[1], "exit", 5);
                chkErr(x, "output to client");
                exit(0);
            }
            else
            {
                char *ptr = strtok(buff_comm, " ");
                if (ptr == NULL)
                {
                    write(fd2[1], "\n", 2);
                    chkErr(x, "output to client");
                }
                else if (strcmp(ptr, "add") == 0)
                {
                    //var
                    int sum = 0;
                    char totalSum[10];
                    bool valid = true;
                    ptr = strtok(NULL, " ");
                    if (ptr == NULL)
                        valid = false;
                    while (ptr != NULL)
                    {
                        if (checkInput(ptr) == 1)
                        {
                            int i = 0;
                            sscanf(ptr, "%d", &i);
                            sum += i;
                            ptr = strtok(NULL, " ");
                        }
                        else
                        {
                            valid = false;
                            break;
                        }
                    }
                    if (valid)
                    {
                        int check = sprintf(totalSum, "%d\n", sum);
                        chkErr(check, "convert sum");
                        //write to client
                        check = write(fd2[1], totalSum, check);
                        chkErr(check, "write sum to client");
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid Input\n", 15);
                        chkErr(x, "output to client");
                    }
                }
                else if (strcmp(ptr, "sub") == 0)
                {
                    //var
                    int sum;
                    char totalSum[10];
                    bool valid = true;
                    ptr = strtok(NULL, " ");
                    if (ptr == NULL)
                        valid = false;
                    if (ptr != NULL && checkInput(ptr) == 1)
                    {
                        int i = 0;
                        sscanf(ptr, "%d", &i);
                        sum = i;
                        ptr = strtok(NULL, " ");
                    }
                    else
                        valid = false;
                    //printf(ptr);
                    while (ptr != NULL && valid)
                    {
                        if (checkInput(ptr) == 1)
                        {
                            int i = 0;
                            sscanf(ptr, "%d", &i);
                            sum -= i;
                            ptr = strtok(NULL, " ");
                        }
                        else
                        {
                            valid = false;
                        }
                    }
                    if (valid)
                    {
                        int check = sprintf(totalSum, "%d\n", sum);
                        chkErr(check, "convert sub");
                        //write to client
                        check = write(fd2[1], totalSum, check);
                        chkErr(check, "write subtraction to client");
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid Input\n", 15);
                        chkErr(x, "output to client");
                    }
                }
                else if (strcmp(ptr, "mul") == 0)
                {
                    //var
                    int prod = 1;
                    char totalProd[10];
                    bool valid = true;
                    ptr = strtok(NULL, " ");
                    if (ptr == NULL)
                        valid = false;
                    while (ptr != NULL && valid)
                    {
                        if (checkInput(ptr) == 1)
                        {
                            int i = 0;
                            sscanf(ptr, "%d", &i);
                            prod *= i;
                            ptr = strtok(NULL, " ");
                        }
                        else
                        {
                            valid = false;
                        }
                    }
                    if (valid)
                    {
                        int check = sprintf(totalProd, "%d\n", prod);
                        chkErr(check, "convert product");
                        //write to client
                        check = write(fd2[1], totalProd, check);
                        chkErr(check, "write prod to client");
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid Input\n", 15);
                        chkErr(x, "output to client");
                    }
                }
                else if (strcmp(ptr, "div") == 0)
                {
                    //var
                    float div = 1;
                    char totalDiv[10];
                    bool valid = true;
                    ptr = strtok(NULL, " ");
                    if (ptr == NULL)
                        valid = false;
                    if (ptr != NULL && checkInput(ptr) == 1)
                    {

                        int i = 0;
                        sscanf(ptr, "%d", &i);
                        div = i;
                        ptr = strtok(NULL, " ");
                    }
                    else
                    {
                        valid = false;
                    }
                    //printf(ptr);
                    while (ptr != NULL && valid)
                    {
                        if (checkInput(ptr) == 1 && checkDiv(ptr) == 1)
                        {
                            int i = 0;
                            sscanf(ptr, "%d", &i);
                            div /= i;
                            ptr = strtok(NULL, " ");
                        }
                        else
                        {
                            valid = false;
                        }
                    }
                    if (valid)
                    {
                        int check = sprintf(totalDiv, "%.2f\n", div);
                        chkErr(check, "convert div");
                        //write to client
                        check = write(fd2[1], totalDiv, check);
                        chkErr(check, "write div to client");
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid Input\n", 15);
                        chkErr(x, "output to client");
                    }
                }
                else if (strcmp(ptr, "run") == 0)
                {
                    ptr = strtok(NULL, " ");
                    if (ptr != NULL)
                    {
                        int pid1 = fork();
                        chkErr(pid,"fork run");
                        if (pid1 == 0)
                        {
                            int chk = execlp(ptr, NULL);
                            chkErr(chk,"exec");
                        }
                        write(fd2[1], "\n", 2);
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid Path\n", 14);
                        chkErr(x, "output to client");
                    }
                }
                else
                {
                    //write to client
                    write(fd2[1], "Command not valid\n", 19);
                    chkErr(x, "output to client");
                }
            }
        }
    }
    return 0;
}