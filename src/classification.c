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

//Programa que lee una imagen y crea un proceso al cual enviar el resultado.
int main (int argc, char **argv){
    /*Se extraen los datos necesarios de argv*/
    int porcentaje, alto, ancho, tamano;

    porcentaje = atoi(argv[1]);
    alto = atoi(argv[3]);
    ancho = atoi(argv[4]);
    tamano = alto * ancho;

    /*Se lee la imagen del pipe y se pasa de int array a int puntero para aplicar la conversión*/
    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgBin = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgBin[c] = imgBuffer[c];
    }

    /*Se clasifica la imagen como nearly black según el porcentaje indicado por el usuario*/
    int nearlyBlack = isNearlyBlack(imgBin, ancho, alto, porcentaje);

    /*Se crea variable para la espera del hijo*/
    int status;

    /*Se crea el pipe para enviar la imagen binarizada al hijo*/
    int pipeImg[2];
    pipe(pipeImg);

    /*Se crea un proceso hijo*/
    pid_t pid = fork();

    if(pid == 0){   //Proceso Hijo
        /*Se cierra la escritura en el pipe y se hace dup2 para permitir la comunicación entre procesos*/
        close(pipeImg[WRITE]);
        dup2(pipeImg[READ], STDIN_FILENO);

        /*Se guarda el resultado como char* para enviarlo como parámetro*/
        char *resultadoStr = malloc(2);
        sprintf(resultadoStr, "%d", nearlyBlack);
        
        /*Se ejecuta process_finish como proceso aparte pasando los parámetros en argv*/
        char *argumentos[] = {argv[0], argv[2], argv[3], argv[4], resultadoStr, NULL};
        execv("./process_finish", argumentos);
    }

    else{   //Proceso Padre
        /*Se convierte la imagen de int* a int array*/
        int imagen[tamano];
        for(int c = 0; c < tamano; c++){
            imagen[c] = imgBin[c];
        }

        /*Se envía la imagen binarizada al hijo para ser guardada*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para avisar al main cuando la imagen sea guardada y el resultado sea entregado por pantalla*/
        waitpid(pid, &status, 0);
    }

    return 0;
}