#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid = fork();
    write(STDIN_FILENO, "hi\n", 3);

    if (pid > 0)
    {
        write(STDIN_FILENO, "Parent\n", 7);
        write(STDIN_FILENO, "hi\n", 3);
    }
    else if (pid == 0)
    {
        write(STDIN_FILENO, "Child\n", 6);
        write(STDIN_FILENO, "hi\n", 3);
    }
    else
        perror("Fork Fail");

    write(STDIN_FILENO, "hi4\n", 4);
    getchar();
    write(STDIN_FILENO, "hi5\n", 4);
}