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

#define READ 0
#define WRITE 1

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

int **leerFiltro(char* nombreArchivo){
    FILE *archivo;
    archivo = fopen(nombreArchivo, "r");
	
	/*Se comprueba la existencia del archivo*/
	if (archivo == NULL){
        printf("Error al leer el filtro\n");
        exit (0);
    }
	
	/*Se crea una matriz dinámica*/
	int i;
	int j;
	int **matriz;
	matriz = (int **) malloc (3*sizeof(int *));
	for (i = 0; i < 3; i++){
		matriz[i] = (int *) malloc (3*sizeof(int));
	}
	
	/*Se agregan los valores del archivo a la matriz*/
	int valor;
	for (j = 0; j < 3; j++){
		for (i = 0; i < 3; i++){
			fscanf(archivo, "%d", &valor);
			matriz[i][j] = valor;
		}
	}

    /*Se retorna la matriz*/
    return matriz;
}

int* imgCharAInt(unsigned char *img, int ancho, int alto, int canales){
    int i;
    int *imagen;
	imagen = (int *) malloc (canales*ancho*alto*sizeof(int));

    int valor;
    unsigned char *valorStr = img;
    for (i = 0; i < canales*ancho*alto; i++){
        valor = (int) *valorStr;
		imagen[i] = valor;
        valorStr++;
	}
    return imagen;
}

int* convertirBN(int *img, int ancho, int alto, int canales){
    /*Se prepara lo necesario para convertir la imagen*/
    size_t tamano = ancho * alto * canales;
    int canalesBN = 1;
    size_t tamanoBN = ancho * alto * canalesBN;

    /*Se asigna memoria para la imagen en blanco y negro*/
    int *imgBN = malloc(tamanoBN*sizeof(int));

    /*Se termina el programa en caso de error*/
    if(imgBN == NULL){
        printf("Error al reservar memoria para la imagen en blanco y negro");
        exit(1);
    }

    /*Ciclo que recorre cada pixel de la imagen a color y convierte los colores según la razón indicada previamente en el enunciado,
    guardando el resultado en imgBN*/
    int i, j;
    for(i = 0, j = 0; i < tamano; i = i + canales, j = j + canalesBN){
        imgBN[j] = ((img[i] * 0.3) + (img[i + 1] * 0.59) + (img[i + 2] * 0.11));
    }

    /*Se retorna la imagen en blanco y negro*/
    return imgBN;
}

int *aplicarFiltro(int *img, int ancho, int alto, char* nombreFiltro){
    /*Se prepara lo necesario para la imagen con filtro*/
    int tamano = ancho * alto;

    /*Se reserva memoria para la imagen con filtro*/
    int *imgFiltro = malloc(tamano*sizeof(int));

    /*Se termina el programa en caso de error*/
    if(imgFiltro == NULL){
        printf("Error al reservar memoria para la imagen con filtro");
        exit(1);
    }

    /*Se obtiene el filtro en una matriz de 3x3*/
    int **filtro = leerFiltro(nombreFiltro);
    
    /*Se crea un entero i usado en el ciclo for*/
    int i;

    /*Ciclo que recorre los pixeles de la imagen en blanco y negro para escribir el filtro aplicado en imgFiltro*/
    for(i = 0; i < tamano; i++){
        /*Condición para que los pixeles que se encuentran en el borde sean escritos sin que se les aplique el filtro*/
        if (i < ancho || i >= (ancho * (alto - 1)) || i % ancho == 0 || (i + 1) % ancho == 0){
            imgFiltro[i] = img[i];
        }

        /*Se aplica el filtro a los pixeles que no se encuentran al borde de la imagen, modificando cada pixel según la suma de
        la multiplicación entre los valores de los pixeles adyacentes y el filtro leído*/
        else{
            imgFiltro[i] = (img[i - (ancho + 1)] * filtro[0][0]) + (img[i - ancho] * filtro[1][0]) + (img[i - (ancho - 1)] * filtro[2][0]) +
                           (img[i - 1] * filtro[0][1]) + (img[i] * filtro[1][1]) + (img[i + 1] * filtro[2][1]) +
                           (img[i + (ancho - 1)] * filtro[0][2]) + (img[i + ancho] * filtro[1][2]) + (img[i + (ancho + 1)] * filtro[2][2]);
            if(imgFiltro[i] < 0){
                imgFiltro[i] = 0;
            }
            else if(imgFiltro[i] > 255){
                imgFiltro[i] = 255;
            }
        }
    }

    /*Se retorna la imagen con el filtro indicado*/
    return imgFiltro;
}

int *binarizar(int *img, int ancho, int alto, int umbral){
    /*Se prepara lo necesario para la imagen binarizada*/
    size_t tamano = ancho * alto;

    /*Se reserva memoria para la imagen binarizada*/
    int *imgBin = malloc(tamano*sizeof(int));

    /*Se termina el programa en caso de error*/
    if(imgBin == NULL){
        printf("Error al reservar memoria para la imagen binarizada");
        exit(1);
    }

    /*Ciclo que recorre los pixeles de la imagen con filtro para binarizarla*/
    int i;
    for(i = 0; i < tamano; i++){
        /*En caso de que el pixel sea mayor que el umbral se deja blanco*/
        if(img[i] > umbral){
            imgBin[i] = 255;
        }

        /*En caso de que el pixel sea igual o menor que el umbral se deja negro*/
        else{
            imgBin[i] = 0;
        }
    }

    /*Se retorna la imagen binarizada*/
    return imgBin;
}

int isNearlyBlack(int *img, int ancho, int alto, int porcentaje){
    /*Se crea suma que contendrá la cantidad de pixeles negros*/
    float suma = 0;

    /*Se prepara lo necesario para recorrer los pixeles de la imagen*/
    size_t tamano = ancho * alto;

    /*Ciclo que recorre la imagen pixel por pixel y guarda la cantidad de pixeles negros (0) encontrados*/
    int i;
    for(i = 0; i < tamano; i++){
        if(img[i] == 0){
            suma = suma + 1;
        }
    }

    /*Se calcula el porcentaje de pixeles negros*/
    float porcentajeNegro = 100 * (suma / (ancho * alto));

    /*Se retorna verdadero en caso de que el porcentaje real de pixeles negros sea igual o mayor al porcentaje indicado*/
    if(porcentajeNegro >= porcentaje){
        return 1;
    }

    /*Se retorna falso en caso de que el porcentaje real de pixeles negros sea menor al porcentaje indicado*/
    else{
        return 0;
    }
}

int escribirImagen(int *img, int alto, int ancho, char *nombreImg){
    int i;
    size_t tamano = ancho * alto;
    unsigned char *imagen = malloc(tamano);
    unsigned char *p = imagen;

    for(i = 0; i < tamano; i++){
        *p = (uint8_t) img[i];
        p += 1;
    }

    /*Escribe una imagen de un solo canal con los parámetros recibidos y sin perder calidad*/
    stbi_write_jpg(nombreImg, ancho, alto, 1, imagen, 100);

    return 1;
}

int main(int argc, char **argv){
    int alto, ancho, canales;
    unsigned char *img = leerJPG("imagenes_entrada/imagen_1.jpg", &ancho, &alto, &canales);
    int *imgInt = imgCharAInt(img, ancho, alto, canales);
    int *imgBN = convertirBN(imgInt, ancho, alto, canales);
    int *imgFiltro = aplicarFiltro(imgBN, ancho, alto, "Filtro.txt");
    int *imgBin = binarizar(imgFiltro, ancho, alto, 30);
    int r = isNearlyBlack(imgBin, ancho, alto, 20);
    escribirImagen(imgBN, alto, ancho, "imagenes_salida/imagen_1.jpg");    
    for(int i = 0; i < alto*ancho; i++){
        //printf("%d ", imgBin[i]);
    }
    printf("\nresultado: %d", r);
}