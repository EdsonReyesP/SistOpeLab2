#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1


int main(int argc, char *argv[]) {
    int pipefd[2];
    char buffer[100];
    pipe(pipefd);

    // this child is generating output to the pipe
    //
    int pid;
    pid = fork();
    if (pid == 0) {
        // attach stdout to the left side of pipe
        // and inherit stdin and stdout from parent
        dup2(pipefd[1],STDOUT_FILENO); //STDOUT_FILENO = un int que tiene el descriptor de stdout.
        close(pipefd[0]);              // not using the right side

        execl("hijo", "ls","-al", NULL);
        printf("HOLA\n");
        perror("exec ls failed");
        exit(EXIT_FAILURE);
    }
    else{
        read(pipefd[READ], buffer, 100);
        printf("Padre: %s\n", buffer);
    }


    return EXIT_SUCCESS;
}