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
    //Almacenar los datos
    int porcentaje, alto, ancho, tamano;

    porcentaje = atoi(argv[1]);
    alto = atoi(argv[3]);
    ancho = atoi(argv[4]);
    tamano = alto * ancho;

    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgBin = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgBin[c] = imgBuffer[c];
    }

    int nearlyBlack = isNearlyBlack(imgBin, ancho, alto, porcentaje);

    /*Se crea variable para la espera del hijo*/
    int status;

    //Crear pipe
    int pipeImg[2];
    pipe(pipeImg);

    //Fork a Conversor en BN
    /*Se crea un proceso hijo*/
    pid_t pid = fork();

    if(pid == 0){
        close(pipeImg[WRITE]);
        dup2(pipeImg[READ], STDIN_FILENO);

        char *resultadoStr = malloc(2);
        sprintf(resultadoStr, "%d", nearlyBlack);
        
        //Ejecuta Binarizacion como proceso aparte pasando los parámetros en argv
        char *argumentos[] = {argv[0], argv[2], argv[3], argv[4], resultadoStr, NULL};
        execv("./process_finish", argumentos);
    }

    //Enviar imagen en blanco y negro por pipe
    else{
        /*Se convierte la imagen de int* a int array*/
        int imagen[tamano];
        for(int c = 0; c < tamano; c++){
            imagen[c] = imgBin[c];
        }

        /*Se envía la imagen leida al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para continuar con la siguiente iteración del for*/
        waitpid(pid, &status, 0);
    }

    return 0;
}