#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t

#define LECTURA 0
#define ESCRITURA 1




int main(int argc, char *argv[]) {
	char mensajeLeido[30];
	read(STDIN_FILENO, mensajeLeido, 30*sizeof(char));
	printf("Mi padre dice: %s\n", mensajeLeido);
	return 0;
}
