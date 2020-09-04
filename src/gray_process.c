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
    /*Se extraen de argv los datos necesarios para transformar la imagen a blanco y negro*/
    int alto, ancho, canales, tamano;

    alto = atoi(argv[5]);
    ancho = atoi(argv[6]);
    canales = atoi(argv[7]);
    tamano = alto * ancho * canales;

    /*Se lee la imagen del pipe y se pasa de int array a int puntero para aplicar la conversión*/
    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgColor = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgColor[c] = imgBuffer[c];
    }

    /*Se convierte la imagen a color a una en blanco y negro*/
    int *imgBN = convertirBN(imgColor, ancho, alto, canales);

    /*Se crea variable para la espera del hijo*/
    int status;

    /*Se crea el pipe para enviar la imagen en blanco y negro*/
    int pipeImg[2];
    pipe(pipeImg);

    /*Se crea un proceso hijo*/
    pid_t pid = fork();

    if(pid == 0){   //Proceso Hijo
        /*Se cierra la escritura del pipe y se hace dup2 para permitir la comunicación entre procesos*/
        close(pipeImg[WRITE]);
        dup2(pipeImg[READ], STDIN_FILENO);
        
        /*Se ejecuta filter como proceso aparte pasando los parámetros en argv*/
        char *argumentos[] = {argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL};
        execv("./filter", argumentos);
    }

    else{   //Proceso Padre
        /*Se convierte la imagen de int* a int array*/
        int tamanoBN = ancho * alto;
        int imagen[tamanoBN];
        for(int c = 0; c < tamanoBN; c++){
            imagen[c] = imgBN[c];
        }

        /*Se envía la imagen en blanco y negro al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para avisar al main cuando el siguiente proceso esté listo*/
        waitpid(pid, &status, 0);
    }

    return 0;
}