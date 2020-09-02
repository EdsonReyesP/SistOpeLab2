#include <unistd.h> //Para utilizar fork(), pipes(), execv(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t

#define LECTURA 0
#define ESCRITURA 1 //variable global 

int *varGlobal; //variable global 


void crearProceso(int *id){
	*id = fork();
}

int main(int argc, char *argv[]) {
	int valorFork; //crea proceso
	if(valorFork < 0){
		exit(-1);
	}
	int status; //variable local
	varGlobal = malloc(sizeof(int)); //heap
	crearProceso(&valorFork);
	if(valorFork == 0){ //hijo
		printf("al parecer soy el hijo y mi pid es: %i\n" , getpid());
		printf("mi padre debería ser el que tiene pid: %i\n", getppid() );
		 
	}
	else{ //padre con vslorFork > 0
		printf("soy el padre y el valor que retorna fork es: %i\n", valorFork );
		printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
		wait(&status);//padre espera el temrino de ejecucion del hijo
	}
	return 0;
}


