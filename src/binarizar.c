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
    int i, umbral, porcentaje, bandera, alto, ancho, tamano;

    i = (int) argv[0];
    umbral = (int) argv[1];
    porcentaje = (int) argv[2];
    bandera = (int) argv[3];
    alto = (int) argv[4];
    ancho = (int) argv[5];
    tamano = alto * ancho;

    int imgBuffer[canales];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgFiltro = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgFiltro[c] = imgBuffer[c];
    }

    int *imgBin = binarizar(imgFiltro, ancho, alto, umbral);

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
        
        //Ejecuta Binarizacion como proceso aparte pasando los parámetros en argv
        char *argumentos[] = {argv[0], argv[2], argv[3], argv[4], argv[5], (const char*) NULL};
        execv("./NearlyBlack", argumentos);
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