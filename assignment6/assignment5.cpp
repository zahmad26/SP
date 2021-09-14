#include <unistd.h>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <list>
#include <fcntl.h>
#include <signal.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int child_pid = -1;
class Process
{
public:
    int pid;
    char pname[50];
    time_t start_time;
    time_t end_time = NULL;
    double elapsed_time;
};
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

void signal_handler(int signo)
{
    int status;
    child_pid = waitpid(0, &status, WNOHANG);
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
        write(STDOUT_FILENO, "Enter commands on the terminal.\nPossible commands:\n****add <list<int>>\n****sub <list<int>>\n****mul <list<int>>\n****div <list<int>>\n****run <path>\n****list\n****list all\n****kill <process name>\n****kill <process id>\n****exit\n", 224);
        while (true)
        {
            int x;
            char command[100] = {};
            char output[1000] = {};
            //take input
            x = write(STDOUT_FILENO, "\n>", 2);
            chkErr(x, "write terminal");
            x = read(STDIN_FILENO, command, 100);
            chkErr(x, "read command");

            //write to server
            x = write(fd1[1], command, strlen(command));
            chkErr(x, "write command to server");

            //read output from server
            x = read(fd2[0], output, 1000);
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
        list<Process> allProcess = {};
        //handle process termination from outside
        signal(SIGCHLD, signal_handler);
        while (true)
        {
            char buff_comm[100] = {};
            char output[100] = {};

            //read from client
            int x = read(fd1[0], buff_comm, 100);
            chkErr(x, "read command from client");

            buff_comm[x - 1] = NULL;

            if (child_pid != -1 && allProcess.empty() == false)
            {
                list<Process>::iterator i;
                for (i = allProcess.begin(); i != allProcess.end(); i++)
                {
                    Process p;
                    p = *i;
                    if (p.end_time == NULL) //active
                    {
                        if (p.pid == child_pid)
                        {
                            (*i).end_time = time(0);
                            (*i).elapsed_time = difftime((*i).end_time, (*i).start_time);
                            child_pid = -1;
                            break;
                        }
                    }
                }
            }
            
                char *ptr = strtok(buff_comm, " ");
                if (ptr == NULL)
                {
                    write(fd2[1], "\n", 2);
                    chkErr(x, "output to client");
                }
                else if (strcmp(buff_comm, "exit") == 0)
                {
                    for (Process p : allProcess)
                    {
                        if (p.end_time == NULL) //active
                        {
                            int kill_stat = kill(p.pid, SIGTERM);
                            chkErr(kill_stat, "exit running programs");
                            int status;
                            int x = waitpid(p.pid, &status, 0);
                            chkErr(x, "wait err");
                        }
                    }
                    write(fd2[1], "exit", 5);
                    chkErr(x, "output to client");
                    exit(0);
                }
                else if (strcmp(ptr, "list") == 0)
                {
                    ptr = strtok(NULL, " ");
                    if (ptr == NULL) //list
                    {
                        char list[1000] = {};
                        if (allProcess.empty() == false)
                        {
                            strcat(list, "pid    pname   start\n");
                            for (Process i : allProcess)
                            {
                                if (i.end_time == NULL) //active
                                {
                                    char b[100] = {};
                                    int check = sprintf(b, "%d  %s   %s\n", i.pid, i.pname, ctime(&i.start_time));
                                    chkErr(check, "sprintf ");
                                    strcat(list, b);
                                }
                            }
                            //write(fd2[1], "here", 5);
                            int x = write(fd2[1], list, strlen(list));
                            chkErr(x, "write output list");
                        }
                        else
                        {
                            int x = write(fd2[1], "\n", 1);
                            chkErr(x, "write output list empty");
                        }
                    }
                    else if (strcmp(ptr, "all") == 0) //list all
                    {
                        if (allProcess.empty() == false)
                        {
                            char list[1000] = {};
                            for (Process i : allProcess)
                            {
                                char b[100] = {};
                                char c[40] = {};
                                if (i.end_time != NULL) // closed
                                {
                                    int check = sprintf(b, "pid  : %d\npname: %s\nstart: %s", i.pid, i.pname, ctime(&i.start_time));
                                    chkErr(check, "sprintf ");
                                    strcat(list, b);
                                    sprintf(c, "end  : %selapse: %fs\n\n", ctime(&i.end_time), i.elapsed_time);
                                    strcat(list, c);
                                }
                                else //active
                                {
                                    int check = sprintf(b, "pid  : %d\npname: %s\nstart: %s", i.pid, i.pname, ctime(&i.start_time));
                                    chkErr(check, "sprintf ");
                                    strcat(list, b);
                                }
                            }
                            int x = write(fd2[1], list, strlen(list));
                            chkErr(x, "write output list all");
                        }
                        else
                        {
                            int x = write(fd2[1], "\n", 1);
                            chkErr(x, "write output list empty");
                        }
                    }
                    else //err
                    {
                        write(fd2[1], "Invalid command", 16);
                        chkErr(x, "output to client");
                    }
                }
                else if (strcmp(ptr, "run") == 0)
                {
                    ptr = strtok(NULL, " ");
                    if (ptr != NULL)
                    {
                        int pipe_status, exec_status, fork_status, pipe_descriptors[2];
                        pipe_status = pipe2(pipe_descriptors, O_CLOEXEC);
                        chkErr(pipe_status, "pipe of run");
                        fork_status = fork();
                        chkErr(fork_status, "fork of run");
                        if (fork_status == 0)
                        {
                            int exec_status = execlp(ptr, ptr, "-s", NULL);
                            if (exec_status == -1)
                            {
                                char error_buffer[4] = {0};
                                int error = errno;
                                sprintf(error_buffer, "%d", error);
                                close(pipe_descriptors[0]);
                                write(pipe_descriptors[1], error_buffer, sizeof(int));
                                close(pipe_descriptors[1]);
                                exit(0);
                            }
                        }
                        if (fork_status > 0)
                        {

                            char buffer[4] = {0};
                            close(pipe_descriptors[1]);
                            int bytes_read = read(pipe_descriptors[0], buffer, sizeof(int));
                            if (bytes_read == 0)
                            {
                                Process p;
                                p.pid = fork_status;
                                strcpy(p.pname, ptr);
                                p.start_time = time(0);
                                allProcess.push_back(p);
                                int x = write(fd2[1], "exec success\n", 14);
                                chkErr(x, "output to client");
                            }
                            else if (bytes_read > 0)
                            {
                                int error = atoi(buffer);
                                char *err = strerror(error);
                                int x = write(fd2[1], err, strlen(err));
                                chkErr(x, "output to client");
                            }
                        }
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid Path\n", 14);
                        chkErr(x, "output to client");
                    }
                }
                else if (strcmp(ptr, "kill") == 0)
                {
                    bool killed = false;
                    ptr = strtok(NULL, " ");
                    if (ptr != NULL)
                    {
                        list<Process>::iterator i;
                        for (i = allProcess.begin(); i != allProcess.end(); i++)
                        {
                            Process p;
                            p = *i;
                            if (p.end_time == NULL)
                            {
                                if (p.pid == atoi(ptr) || strcmp(ptr, p.pname) == 0)
                                {

                                    int status;
                                    int kill_status = kill(p.pid, SIGTERM);
                                    chkErr(kill_status, "Could not kill");
                                    if (kill_status == 0)
                                    {
                                        waitpid(p.pid, &status, 0);
                                        killed = true;
                                        (*i).end_time = time(0);
                                        (*i).elapsed_time = difftime((*i).end_time, (*i).start_time);
                                        write(fd2[1], "\n", 1);
                                        break;
                                    }
                                }
                            }
                        }
                        if (!killed)
                        {
                            //write to client
                            int x = write(fd2[1], "Invalid pid or pname\n", 22);
                            chkErr(x, "output to client");
                        }
                    }
                    else
                    {
                        //write to client
                        write(fd2[1], "Invalid process id or name\n", 28);
                        chkErr(x, "output to client");
                    }
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
                else
                {
                    //write to client
                    write(fd2[1], "Command not valid\n", 19);
                    chkErr(x, "output to client");
                }
            
        }
    }
    return 0;
}