#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "funciones.h"

int main (int argc, char **argv){
    int cantImg;
    int umbral;
    int porcentaje;
    int bandera = 0;
    char* filtro = NULL;
    recibirDatos(argc, argv, &cantImg, &umbral, &porcentaje, &filtro, &bandera);
    if(bandera == 1){
        printf("\n------------------------------------\n|    image    |    nearly black    |\n|-------------|--------------------|\n");
    }
    
    for(int i = 1; i <= cantImg; i++){
        int ancho, alto, canales;
        char* nombre = malloc(32);
        sprintf(nombre, "imagenes_entrada/imagen_%d.jpg", i);
        unsigned char *img = leerJPG(nombre, &ancho, &alto, &canales);
        unsigned char *imgBN = convertirBN(img, &ancho, &alto, &canales);
        unsigned char *imgFiltro = aplicarFiltro(imgBN, &ancho, &alto, filtro);
        unsigned char *imgBin = binarizar(imgFiltro, &ancho, &alto, umbral);
        char* salida = malloc(32);
        sprintf(salida, "imagenes_salida/imagen_%d.jpg", i);
        escribirImgBN(salida, ancho, alto, imgBin);
        int clasificacion = isNearlyBlack(imgBin, &ancho, &alto, porcentaje);
        if(bandera == 1 && clasificacion == 1){
            printf("|  imagen_%d   |         yes        |\n", i);
        }
        else if(bandera == 1 && clasificacion == 0){
            printf("|  imagen_%d   |         no         |\n", i);
        }
    }

    if(bandera == 1){
        printf("------------------------------------\n\n");
    }
}