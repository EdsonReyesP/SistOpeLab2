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
    int alto, ancho, tamano;
    char* filtro;

    filtro = argv[4];
    alto = atoi(argv[5]);
    ancho = atoi(argv[6]);
    tamano = alto * ancho;

    /*Se lee la imagen del pipe y se pasa de int array a int puntero para aplicar el filtro*/
    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgBN = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgBN[c] = imgBuffer[c];
    }

    /*Se aplica el filtro según la máscara indicada previamente por el usuario*/
    int *imgFiltro = aplicarFiltro(imgBN, ancho, alto, filtro);

    /*Se crea variable para la espera del hijo*/
    int status;

    /*Se crea el pipe para enviar la imagen al hijo*/
    int pipeImg[2];
    pipe(pipeImg);

    /*Se crea un proceso hijo*/
    pid_t pid = fork();

    if(pid == 0){   //Proceso Hijo
        close(pipeImg[WRITE]);
        dup2(pipeImg[READ], STDIN_FILENO);
        
        /*Se ejecuta binary como proceso aparte pasando los parámetros en argv*/
        char *argumentos[] = {argv[0], argv[1], argv[2], argv[3], argv[5], argv[6], NULL};
        execv("./binary", argumentos);
    }

    else{   //Proceso Padre
        /*Se convierte la imagen de int* a int array*/
        int imagen[tamano];
        for(int c = 0; c < tamano; c++){
            imagen[c] = imgFiltro[c];
        }

        /*Se envía la imagen con el filtro aplicado al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para avisar al main cuando el siguiente proceso esté listo*/
        waitpid(pid, &status, 0);
    }

    return 0;
}