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
    int umbral, alto, ancho, tamano;

    umbral = atoi(argv[1]);
    alto = atoi(argv[4]);
    ancho = atoi(argv[5]);
    tamano = alto * ancho;

    /*Se lee la imagen del pipe y se pasa de int array a int puntero para aplicar la binarización*/
    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgFiltro = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgFiltro[c] = imgBuffer[c];
    }

    /*Se binariza la imagen según el umbral indicado por el usuario previamente*/
    int *imgBin = binarizar(imgFiltro, ancho, alto, umbral);

    /*Se crea variable para la espera del hijo*/
    int status;

    /*Se crea el pipe para enviar la imagen al siguiente proceso*/
    int pipeImg[2];
    pipe(pipeImg);

    /*Se crea un proceso hijo*/
    pid_t pid = fork();

    if(pid == 0){   //Proceso Hijo
        /*Se cierra la lectura del pipe y se hace dup2 para permitir la comunicación*/
        close(pipeImg[WRITE]);
        dup2(pipeImg[READ], STDIN_FILENO);
        
        /*Se ejecuta classification como proceso aparte pasando los parámetros en argv*/
        char *argumentos[] = {argv[0], argv[2], argv[3], argv[4], argv[5], NULL};
        execv("./classification", argumentos);
    }

    else{   //Proceso Padre
        /*Se convierte la imagen de int* a int array*/
        int imagen[tamano];
        for(int c = 0; c < tamano; c++){
            imagen[c] = imgBin[c];
        }

        /*Se envía la imagen binarizada al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para continuar con la siguiente iteración del for*/
        waitpid(pid, &status, 0);
    }

    return 0;
}