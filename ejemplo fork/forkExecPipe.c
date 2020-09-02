#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t

#define LECTURA 0
#define ESCRITURA 1


int main(int argc, char *argv[]) {
	int valorFork;
	char fdRead[100];
	char* argumentos[2] = {"./salida2", NULL};
	char mensajeEscrito[30] = "Hola hijo, ¿como estas?";
	char *mensajeLeido = (char*)malloc(sizeof(char)*30);
	int *pipes = (int*)malloc(sizeof(int)*2);
	pipe(pipes); //inicializa el pipe

	int status;
    pid_t pid;
	

	pid = fork(); 
	if(pid > 0){
		close(pipes[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
		write(pipes[ESCRITURA], mensajeEscrito, 30*sizeof(char));
		printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
		waitpid(pid, &status,0);
	}
	else{
		
		close(pipes[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
		printf("al parecer soy el hijo y mi pid es: %i\n" , getpid());
		printf("mi padre debería ser el que tiene pid: %i\n", getppid() );
		dup2(pipes[LECTURA], STDIN_FILENO);

		execv(argumentos[0], argumentos);
	}

}


