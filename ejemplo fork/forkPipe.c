#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t

#define LECTURA 0
#define ESCRITURA 1



void crearProceso(int *id){
	*id = fork();

}
int main(int argc, char *argv[]) {
	int valorFork;
	char mensajeEscrito[30] = "Hola hijo, ¿como estas?";
	char *mensajeLeido = (char*)malloc(sizeof(char)*30);
	int *pipes = (int*)malloc(sizeof(int)*2);
	pipe(pipes); //inicializa el pipe
	printf("Pipes tiene los siguientes valores: %d, %d\n", pipes[LECTURA], pipes[ESCRITURA]);
	

	crearProceso(&valorFork);
	if(valorFork == 0){
		close(pipes[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
		printf("al parecer soy el hijo y mi pid es: %i\n" , getpid());
		printf("mi padre debería ser el que tiene pid: %i\n", getppid() );
		read(pipes[LECTURA], mensajeLeido, 30*sizeof(char));
		printf("Mi padre dice: %s\n", mensajeLeido);
	}
	else{
		close(pipes[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
		write(pipes[ESCRITURA], mensajeEscrito, 30*sizeof(char));
		printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
	}

}


