#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "funciones.h"

//Programa que lee una imagen jpeg a colores y la convierte en una imagen binarizada que escribe en memoria, además de clasificarla como nearly black o no
int main (int argc, char **argv){
    /*Se crean valores donde se almacenarán los datos ingresados por el usuario*/
    int cantImg;
    int umbral;
    int porcentaje;
    int bandera = 0;
    char* filtro = NULL;

    /*Se almacenan los datos ingresados por el usuario*/
    recibirDatos(argc, argv, &cantImg, &umbral, &porcentaje, &filtro, &bandera);

    /*Se escribe por pantalla la cabecera de la tabla de clasificación en caso de ser requerido*/
    if(bandera == 1){
        printf("\n------------------------------------\n|    image    |    nearly black    |\n|-------------|--------------------|\n");
    }
    
    /*Ciclo que se repite la cantidad de veces que indique el usuario*/
    for(int i = 1; i <= cantImg; i++){
        /*Se crean los datos necesarios para cada iteración*/
        int ancho, alto, canales;
        char* nombre = malloc(32);
        sprintf(nombre, "imagenes_entrada/imagen_%d.jpg", i);

        /*Se lee la imagen_i*/
        unsigned char *img = leerJPG(nombre, &ancho, &alto, &canales);

        /*Se aplican las conversiones indicadas en el enunciado*/
        unsigned char *imgBN = convertirBN(img, &ancho, &alto, &canales);
        unsigned char *imgFiltro = aplicarFiltro(imgBN, &ancho, &alto, filtro);
        unsigned char *imgBin = binarizar(imgFiltro, &ancho, &alto, umbral);

        /*Se escribe en memoria la imagen_i*/
        char* salida = malloc(32);
        sprintf(salida, "imagenes_salida/imagen_%d.jpg", i);
        escribirImgBN(salida, ancho, alto, imgBin);

        /*Se clasifica la imagen_i en nearly black y se guarda en clasificación*/
        int clasificacion = isNearlyBlack(imgBin, &ancho, &alto, porcentaje);

        /*Se muestra por pantalla si la imagen_i es nearly black o no en caso de ser requerido*/
        if(bandera == 1 && clasificacion == 1){
            printf("|  imagen_%d   |         yes        |\n", i);
        }
        else if(bandera == 1 && clasificacion == 0){
            printf("|  imagen_%d   |         no         |\n", i);
        }
    }

    /*Se cierra la tabla de clasificación en caso de ser requerido*/
    if(bandera == 1){
        printf("------------------------------------\n\n");
    }
}