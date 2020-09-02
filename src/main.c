#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "funciones.h"

#define READ 0
#define WRITE 1

//Programa que lee una imagen jpeg a colores y la convierte en una imagen binarizada que escribe en memoria, además de clasificarla como nearly black o no
int main (int argc, char **argv){
    /*Se crean valores donde se almacenarán los datos ingresados por el usuario*/
    int cantImg;
    int umbral;
    int porcentaje;
    int bandera = 0;
    char* filtro = NULL;

    /*Se crea variable para la espera del hijo*/
    int status;

    /*Se almacenan los datos ingresados por el usuario*/
    recibirDatos(argc, argv, &cantImg, &umbral, &porcentaje, &filtro, &bandera);

    /*Se escribe por pantalla la cabecera de la tabla de clasificación en caso de ser requerido*/
    if(bandera == 1){
        printf("\n------------------------------------\n|    image    |    nearly black    |\n|-------------|--------------------|\n");
    }
    
    /*Ciclo que se repite la cantidad de veces que indique el usuario*/
    for(int i = 1; i <= cantImg; i++){

        int pipefd[2];
        //char buffer[100];
        pipe(pipefd);

        /*Se crea un proceso hijo*/
        pid_t pid = fork();

        if(pid == 0){   //Proceso Hijo
            close(pipefd[WRITE]);
            dup2(pipefd[READ],STDIN_FILENO); //STDOUT_FILENO = un int que tiene el descriptor de stdout.
            char* iStr = malloc(3);
            char* umbralStr = malloc(10);
            char* porcentajeStr = malloc(10);
            char* banderaStr = malloc(10);
            sprintf(iStr, "%d", i);
            sprintf(umbralStr, "%d", umbral);
            sprintf(porcentajeStr, "%d", porcentaje);
            sprintf(banderaStr, "%d", bandera);


            //Ejecuta Lectura como proceso aparte recibiendo parámetros en arcv
            char *argumentos[] = {iStr, umbralStr, porcentajeStr, banderaStr, filtro, (const char*) NULL};
            execv("./Lectura", argumentos);
        }
        else{           //Proceso Padre
            /*Se espera al hijo para continuar con la siguiente iteración del for*/
            int img[5] = {1, 2, 3, 4, 5};
            close(pipefd[READ]);
            write(pipefd[WRITE], img, sizeof(img));
            waitpid(pid, &status, 0);
        }
    }

    /*Se cierra la tabla de clasificación en caso de ser requerido*/
    if(bandera == 1){
        printf("------------------------------------\n\n");
    }
}