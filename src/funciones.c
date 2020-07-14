#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"


void recibirDatos(int argc, char **argv, int *cantImg, int *umbralBin, int *porcentaje, char* *nombreMasc, int *bandera){
    int c;
    while ((c = getopt (argc, argv, "c:u:n:m:b")) != -1){
        switch (c)
        {
        case 'c':
            sscanf(optarg, "%d", cantImg);
            if(*cantImg <= 0){
                *cantImg = 1;
            }
            break;
        
        case 'u':
            sscanf(optarg, "%d", umbralBin);
            break;

        case 'n':
            sscanf(optarg, "%d", porcentaje);
            if(*porcentaje > 100){
                *porcentaje = 100;
            }
            else if(*porcentaje < 0){
                *porcentaje = 0;
            }
            break;

        case 'm':
            *nombreMasc = optarg;
            break;
        
        case 'b':
            *bandera = 1;
            break;

        case '?':

            if (optopt == 'c'){
                fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
            }

            else if(optopt == 'u'){
                fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
            }

            else if(optopt == 'n'){
                fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
            }

            else if(optopt == 'm'){
                fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
            }
            
            else if (isprint (optopt))
            fprintf(stderr, "Opcion desconocida `-%c'.\n", optopt);

            else
            fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);

        default:
            abort ();
        }
    }
}

unsigned char *leerJPG(char *nombreImg, int *ancho, int *alto, int *canales){
    unsigned char *img = stbi_load(nombreImg, ancho, alto, canales, 0);
    if(img == NULL){
        printf("Error al cargar la imagen\n");
        exit(1);
    }
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

unsigned char *convertirBN(unsigned char *img, int *ancho, int *alto, int *canales){
    size_t tamano = *ancho * *alto * *canales;
    int canalesBN = 1;
    size_t tamanoBN = *ancho * *alto * canalesBN;

    unsigned char *imgBN = malloc(tamanoBN);
    if(imgBN == NULL){
        printf("Error al reservar memoria para la imagen en blanco y negro");
        exit(1);
    }

    for(unsigned char *p = img, *pg = imgBN; p < img + tamano; p += *canales, pg += canalesBN){
        *pg = (uint8_t)((*p * 0.3) + (*(p + 1) * 0.59) + (*(p + 2) * 0.11));
    }

    return imgBN;
}

unsigned char *aplicarFiltro(unsigned char *img, int *ancho, int *alto, char* nombreFiltro){
    int canalesBN = 1;
    size_t tamanoBN = *ancho * *alto * canalesBN;
    int canalesFiltro = canalesBN;
    size_t tamanoFiltro = *ancho * *alto * canalesFiltro;

    unsigned char *imgFiltro = malloc(tamanoFiltro);
    if(imgFiltro == NULL){
        printf("Error al reservar memoria para la imagen con filtro");
        exit(1);
    }
    int pgInt = 0;

    int **filtro = leerFiltro(nombreFiltro);

    for(unsigned char *pbn = img, *pf = imgFiltro; pbn < img + tamanoBN; pf += canalesFiltro, pbn += canalesBN, pgInt += canalesBN){
        if (pgInt < (*ancho * canalesBN) || pgInt >= (*ancho * (*alto - 1) * canalesBN) || pgInt % (*ancho * canalesBN) == 0 || (pgInt + 1) % (*ancho * canalesBN) == 0){
            *pf = *pbn;
        }
        else{
            *pf = (uint8_t) ((*(pbn - ((*ancho + 1) * canalesBN)) * filtro[0][0]) + (*(pbn - ((*ancho) * canalesBN)) * filtro[1][0]) + (*(pbn - ((*ancho - 1) * canalesBN)) * filtro[2][0]) +
                            (*(pbn - canalesBN) * filtro[0][1]) + (*pbn * filtro[1][1]) + (*(pbn + canalesBN) * filtro[2][1]) +
                            (*(pbn + ((*ancho - 1) * canalesBN)) * filtro[0][2]) + (*(pbn + ((*ancho) * canalesBN)) * filtro[1][2]) + (*(pbn + ((*ancho + 1) * canalesBN)) * filtro[2][2]));
        }
    }

    return imgFiltro;
}

unsigned char *binarizar(unsigned char *img, int *ancho, int *alto, int umbral){
    int canalesFiltro = 1;
    size_t tamanoFiltro = *ancho * *alto * canalesFiltro;
    int canalesBin = canalesFiltro;
    size_t tamanoBin = *ancho * *alto * canalesBin;

    unsigned char *imgBin = malloc(tamanoBin);
    if(imgBin == NULL){
        printf("Error al reservar memoria para la imagen binarizada");
        exit(1);
    }

    for(unsigned char *pf = img, *pb = imgBin; pf < img + tamanoFiltro; pf += canalesFiltro, pb += canalesBin){
        if(*pf > umbral){
            *pb = 255;
        }
        else{
            *pb = 0;
        }
    }

    return imgBin;
}

int isNearlyBlack(unsigned char *img, int *ancho, int *alto, int porcentaje){
    float suma = 0;
    int canalesBin = 1;
    size_t tamanoBin = *ancho * *alto * canalesBin;

    for(unsigned char *pb = img; pb < img + tamanoBin; pb += canalesBin){
        if(*pb == 0){
            suma = suma + 1;
        }
    }

    float porcentajeNegro = 100 * (suma / (*ancho * *alto));
    if(porcentajeNegro >= porcentaje){
        return 1;
    }
    else{
        return 0;
    }
}

void escribirImgBN(char* nombreImg, int ancho, int alto, unsigned char *imagen){
    stbi_write_jpg(nombreImg, ancho, alto, 1, imagen, 100);
}