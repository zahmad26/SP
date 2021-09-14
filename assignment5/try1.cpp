#include <unistd.h>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <list>

using namespace std;

class Process
{
public:
    int pid;
    char* pname;
};

int main()
{
    list<Process> p;

    Process p1;
    p1.pid = 1;
    p1.pname = "p1";
    p.push_back(p1);
    Process p2;
    p2.pid = 2;
    p2.pname = "p2";
    p.push_back(p2);
    char list[500] = {};
    strcat(list,"pid    pname\n");
    for (Process i : p)
    {
        char b[50];
        sprintf(b, "%d      %s\n", i.pid, i.pname);
        strcat(list, b);
    }

        write(STDOUT_FILENO, list, strlen(list));
}