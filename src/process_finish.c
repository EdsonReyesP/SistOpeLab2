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
    int i, bandera, alto, ancho, tamano, resultado;

    i = atoi(argv[0]);
    bandera = atoi(argv[1]);
    alto = atoi(argv[2]);
    ancho = atoi(argv[3]);
    resultado = atoi(argv[4]);
    tamano = alto * ancho;

    /*Se lee la imagen del pipe y se pasa de int array a int puntero para almacenar la imagen en memoria*/
    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgBin = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgBin[c] = imgBuffer[c];
    }

    /*Se genera el nombre de la imagen a almacenar según el número de la iteración*/
    char* nombreImg = malloc(32);
    sprintf(nombreImg, "imagenes_salida/imagen_%d.jpg", i);

    /*Se escribe la imagen en memoria*/
    escribirImagen(imgBin, alto, ancho, nombreImg);

    /*Se muestra por pantalla si la imagen_i es nearly black o no en caso de ser requerido por la bandera*/
    if(bandera == 1 && resultado == 1){
        printf("|  imagen_%d   |         yes        |\n", i);
    }
    else if(bandera == 1 && resultado == 0){
        printf("|  imagen_%d   |         no         |\n", i);
    }

    return 0;
}