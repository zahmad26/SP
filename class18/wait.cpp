
#include <unistd.h>
#include <stdio.h>

int main(){
    int pid = fork();
    if(pid == -1){
        perror("fork fail");
    }else if(pid == 0){

        write(STDOUT_FILENO, "Child",5);
        //exit(0);
    } else{

    }
}