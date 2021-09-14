#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h> // perr
#include <ctime>
#include <iostream>

void copy()
{
    int x = open("./orignal.txt", O_RDONLY);
    if (x == -1)
    {
        perror("x file open: ");
    }
    int y = open("./copy.txt", O_CREAT | O_RDWR, S_IRWXU);
    if (y == -1)
    {
        perror("y file open: ");
    }

    time_t start_time;
    time_t end_time;
    char buff[1000];
    int l = read(x, buff, 1000);
    time(&start_time);
    printf(ctime(&start_time));
    while (l != 0)
    {
        int r = write(y, buff, l);
        if (r <= 0)
        {
            write(STDOUT_FILENO, "could not write", 15);
        }
        l = read(x, buff, 1000);
    }
    time(&end_time);
    printf(ctime(&end_time));
    double elapsed_time = difftime(end_time, start_time);
    printf("duration: %.0f seconds\n", elapsed_time);
}

void copyfsync()
{
    int x = open("./orignal.txt", O_RDONLY);
    if (x == -1)
    {
        perror("x file open: ");
    }
    int y = open("./copy1.txt", O_CREAT | O_RDWR, S_IRWXU);
    if (y == -1)
    {
        perror("y file open: ");
    }

    time_t start_time;
    time_t end_time;
    char buff[1000];
    int l = read(x, buff, 1000);
    time(&start_time);
    printf(ctime(&start_time));
    while (l != 0)
    {
        int r = write(y, buff, l);
        fsync(y);
        if (r <= 0)
        {
            write(STDOUT_FILENO, "could not write", 15);
        }
        l = read(x, buff, 1000);
    }
    time(&end_time);
    printf(ctime(&end_time));
    double elapsed_time = difftime(end_time, start_time);
    printf("duration: %.0f seconds\n", elapsed_time);
}

void copyfdatasync()
{
    int x = open("./orignal.txt", O_RDONLY);
    if (x == -1)
    {
        perror("x file open: ");
    }
    int y = open("./copy2.txt", O_CREAT | O_RDWR, S_IRWXU);
    if (y == -1)
    {
        perror("y file open: ");
    }

    time_t start_time = NULL;
    time_t end_time = NULL;
    char buff[1000];
    int l = read(x, buff, 1000);
    start_time = time(0);
    printf(ctime(&start_time));
    while (l != 0)
    {
        int r = write(y, buff, l);
        fdatasync(y);
        if (r <= 0)
        {
            write(STDOUT_FILENO, "could not write", 15);
        }
        l = read(x, buff, 1000);
    }
    end_time = time(0);
    printf(ctime(&end_time));
    double elapsed_time = difftime(end_time, start_time);
    printf("duration: %.0f seconds\n", elapsed_time);
}
void copyosync()
{
    int x = open("./orignal.txt", O_RDONLY);
    if (x == -1)
    {
        perror("x file open: ");
    }
    int y = open("./copy3.txt", O_CREAT | O_RDWR | O_SYNC, S_IRWXU);
    if (y == -1)
    {
        perror("y file open: ");
    }

    time_t start_time = NULL;
    time_t end_time = NULL;
    char buff[1000];
    int l = read(x, buff, 1000);
    start_time = time(0);
    printf(ctime(&start_time));
    while (l != 0)
    {
        int r = write(y, buff, l);
        if (r <= 0)
        {
            write(STDOUT_FILENO, "could not write", 15);
        }
        l = read(x, buff, 1000);
    }
    end_time = time(0);
    printf(ctime(&end_time));
    double elapsed_time = difftime(end_time, start_time);
    printf("duration: %.0f seconds\n", elapsed_time);
}
void copy1byte()
{
    int x = open("./orignal.txt", O_RDONLY);
    if (x == -1)
    {
        perror("x file open: ");
    }
    int y = open("./copy4.txt", O_CREAT | O_RDWR, S_IRWXU);
    if (y == -1)
    {
        perror("y file open: ");
    }

    time_t start_time = NULL;
    time_t end_time = NULL;
    char buff[1];
    int l = read(x, buff, 1);
    start_time = time(0);
    printf(ctime(&start_time));
    while (l != 0)
    {
        int r = write(y, buff, l);
        fsync(y);
        if (r <= 0)
        {
            write(STDOUT_FILENO, "could not write", 15);
        }
        l = read(x, buff, 1);
    }
    end_time = time(0);
    printf(ctime(&end_time));
    double elapsed_time = difftime(end_time, start_time);
    printf("duration: %.0f seconds\n", elapsed_time);
}
int main()
{
    printf("copy w/o sync:\n");
    copy();
    printf("\ncopy using fsync:\n");
    copyfsync();
    printf("\ncopy using fdatasync:\n");
    copyfdatasync();
    printf("\ncopy using O_SYNC flag:\n");
    copyosync();
    printf("\ncopy using 1 byte:\n");
    copy1byte();

}
