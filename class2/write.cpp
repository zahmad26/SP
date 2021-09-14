#include <unistd.h>

int main()
{
    write(STDOUT_FILENO, "Hello",5);
}