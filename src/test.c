#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

unsigned char *leerJPG(char *nombreImg, int *ancho, int *alto, int *canales){
    /*Se crea img con la funcion stbi_load para cargar la imagen en este y los datos en los punteros adecuados*/
    unsigned char *img = stbi_load(nombreImg, ancho, alto, canales, 0);

    /*Condición de salida en caso de que la imagen no pueda ser leída o no exista*/
    if(img == NULL){
        printf("Error al cargar la imagen\n");
        exit(1);
    }

    /*Se retorna img con la información leída de la imagen*/
    return img;
}

int** imgCharAInt(unsigned char *img, int alto, int ancho, int canales){
    int i;
    int **matriz;
	matriz = (int **) malloc (alto*sizeof(int *));
    for (i = 0; i < alto; i++){
		matriz[i] = (int *) malloc (canales*ancho*sizeof(int));
	}

    int j;
    int c;
    int valor;
    unsigned char *valorStr = img;
    for (j = 0; j < alto; j++){
		for (i = 0; i < ancho; i++){
            for (c = 0; c < canales; c++){
                valor = (int) *valorStr;
			    matriz[i+c][j] = valor;
                printf("%d ", valor);
                valorStr++;
            }
		}
	}
    return matriz;
}

int main(int argc, char **argv){
    int alto, ancho, canales;
    unsigned char *img = leerJPG("imagenes_entrada/imagen_1.jpg", &alto, &ancho, &canales);
    imgCharAInt(img, alto, ancho, canales);
}