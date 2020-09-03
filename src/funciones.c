#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

//ENTRADAS:         Valores recibidos en la función main (argc que contiene la cantidad y argv que contiene el string indicado al
//                  ejecutar el programa).
//                  Puntero a int cantImg donde se guarda la cantidad de imágenes a procesar; Puede ser nulo.
//                  Puntero a int umbralBin donde se guarda el umbral para convertir la imagen a binario; Puede ser nulo.
//                  Puntero a int porcentaje donde se guarda el umbral para clasificar la imagen en nearly black; Puede ser nulo.
//                  Puntero a char* nombreMasc donde se guarda el nombre del archivo con la máscara a usar como filtro; Puede ser nulo.
//                  Puntero a int bandera donde se guarda la bandera que indica si la clasificación aparece por pantalla.
//
//FUNCIONAMIENTO:   Esta función void guarda los datos recibidos por el usuario al ejecutar el programa, los cuales vienen dentro de
//                  argv y son guardados en los punteros ingresados según corresponda. Esto gracias a la librería getopt.h.
//
//SALIDAS:          Al ser una función void no tiene salida, pero los punteros cantImg, umbralBin, porcentaje, nombreMasc y bandera se
//                  reemplazan con los valores indicados por el usuario que vienen guardados en argv 
void recibirDatos(int argc, char **argv, int *cantImg, int *umbralBin, int *porcentaje, char* *nombreMasc, int *bandera){
    int c;
    //Se utiliza getopt para guardar los valores ingresados por el usuario según corresponda.
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

//ENTRADAS:         char* nombreImg que indica el nombre del archivo a ser leído.
//                  Puntero a int ancho donde se guarda el ancho de la imagen.
//                  Puntero a int alto donde se guarda el alto de la imagen.
//                  Puntero a int canales donde se guarda la cantidad de canales de color de la imagen.
//
//FUNCIONAMIENTO:   Se hace uso de la funcion stbi_load() de la librería stb para manipular imágenes jpeg para obtener un unsigned char*
//                  con la información de los pixeles de la imagen indicada en nombreImg, además de los datos de cabezera en esta imagen,
//                  los que son almacenados en los punteros indicados como parámetros de entrada.
//
//SALIDAS:          La función devuelve un unsigned char* con los pixeles de la imagen previamente indicada.
//                  También guarda el ancho, alto y canales de color de la imagen en los respectivos punteros.
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

//ENTRADAS:         char* nombreArchivo que indica el nombre del archivo con el filtro que se desea abrir; debe incluir la extensión.
//
//FUNCIONAMIENTO:   Función que se encarga de guardar el contenido del filtro de 3x3 usado para una de las conversiones de la imagen
//                  en una matriz dinámica para poder obtener los datos más tarde con facilidad.
//
//SALIDAS:          Matriz dinámica de 3x3 con los enteros leídos desde el archivo.
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

//ENTRADAS:         unsigned char* img con los pixeles de la imagen a color
//                  Puntero a entero ancho con el ancho de la imagen.
//                  Puntero a entero alto con el alto de la imagen.
//                  Puntero a entero canales con los canales de la imagen a color.
//
//FUNCIONAMIENTO:   Función que toma una imagen a color y la convierte a blanco y negro al tomar una porción de cada color (RGB) y
//                  juntarlos en un solo canal de grises.
//
//SALIDAS:          unsigned char* con la imagen en blanco y negro.
unsigned char *convertirBN(unsigned char *img, int *ancho, int *alto, int *canales){
    /*Se prepara lo necesario para convertir la imagen*/
    size_t tamano = *ancho * *alto * *canales;
    int canalesBN = 1;
    size_t tamanoBN = *ancho * *alto * canalesBN;

    /*Se asigna memoria para la imagen en blanco y negro*/
    unsigned char *imgBN = malloc(tamanoBN);

    /*Se termina el programa en caso de error*/
    if(imgBN == NULL){
        printf("Error al reservar memoria para la imagen en blanco y negro");
        exit(1);
    }

    /*Ciclo que recorre cada pixel de la imagen a color y convierte los colores según la razón indicada previamente en el enunciado,
    guardando el resultado en imgBN*/
    for(unsigned char *p = img, *pg = imgBN; p < img + tamano; p += *canales, pg += canalesBN){
        *pg = (uint8_t)((*p * 0.3) + (*(p + 1) * 0.59) + (*(p + 2) * 0.11));
    }

    /*Se retorna la imagen en blanco y negro*/
    return imgBN;
}


//ENTRADAS:         unsigned char* img con los pixeles de la imagen en blanco y negro.
//                  Puntero a entero ancho con el ancho de la imagen.
//                  Puntero a entero alto con el alto de la imagen.
//                  char* nombreFiltro con el nombre del archivo que posee el filtro a aplicar a la imagen.
//
//FUNCIONAMIENTO:   Función lee y guarda el filtro indicado en nombreFiltro para luego aplicarlo a cada pixel de la imagen en blanco y
//                  negro a excepción de los pixeles en los bordes que se mantienen tal como están según lo indicado.
//
//SALIDAS:          unsigned char con la imagen en blanco y negro con el filtro ya aplicado.
unsigned char *aplicarFiltro(unsigned char *img, int *ancho, int *alto, char* nombreFiltro){
    /*Se prepara lo necesario para la imagen con filtro*/
    int canalesBN = 1;
    size_t tamanoBN = *ancho * *alto * canalesBN;
    int canalesFiltro = canalesBN;
    size_t tamanoFiltro = *ancho * *alto * canalesFiltro;

    /*Se reserva memoria para la imagen con filtro*/
    unsigned char *imgFiltro = malloc(tamanoFiltro);

    /*Se termina el programa en caso de error*/
    if(imgFiltro == NULL){
        printf("Error al reservar memoria para la imagen con filtro");
        exit(1);
    }

    /*Se obtiene el filtro en una matriz de 3x3*/
    int **filtro = leerFiltro(nombreFiltro);
    
    /*Se crea un entero pgInt usado en el ciclo for*/
    int pgInt = 0;

    /*Ciclo que recorre los pixeles de la imagen en blanco y negro para escribir el filtro aplicado en imgFiltro*/
    for(unsigned char *pbn = img, *pf = imgFiltro; pbn < img + tamanoBN; pf += canalesFiltro, pbn += canalesBN, pgInt += canalesBN){
        /*Condición para que los pixeles que se encuentran en el borde sean escritos sin que se les aplique el filtro*/
        if (pgInt < (*ancho * canalesBN) || pgInt >= (*ancho * (*alto - 1) * canalesBN) || pgInt % (*ancho * canalesBN) == 0 || (pgInt + 1) % (*ancho * canalesBN) == 0){
            *pf = *pbn;
        }

        /*Se aplica el filtro a los pixeles que no se encuentran al borde de la imagen, modificando cada pixel según la suma de
        la multiplicación entre los valores de los pixeles adyacentes y el filtro leído*/
        else{
            *pf = (uint8_t) ((*(pbn - ((*ancho + 1) * canalesBN)) * filtro[0][0]) + (*(pbn - ((*ancho) * canalesBN)) * filtro[1][0]) + (*(pbn - ((*ancho - 1) * canalesBN)) * filtro[2][0]) +
                            (*(pbn - canalesBN) * filtro[0][1]) + (*pbn * filtro[1][1]) + (*(pbn + canalesBN) * filtro[2][1]) +
                            (*(pbn + ((*ancho - 1) * canalesBN)) * filtro[0][2]) + (*(pbn + ((*ancho) * canalesBN)) * filtro[1][2]) + (*(pbn + ((*ancho + 1) * canalesBN)) * filtro[2][2]));
        }
    }

    /*Se retorna la imagen con el filtro indicado*/
    return imgFiltro;
}

//ENTRADAS:         unsigned char* img con los pixeles de la imagen en blanco y negro con el filtro aplicado.
//                  Puntero a entero ancho con el ancho de la imagen.
//                  Puntero a entero alto con el alto de la imagen.
//                  int umbral con el valor entero en que se diferencia negro de blanco; puede ser negativo o positivo.
//
//FUNCIONAMIENTO:   Función que toma una imagen en blanco y negro para convertirla en una imagen binarizada según el umbral
//                  que se indique, siendo blanco cada pixel que lo exceda y negro los que no.
//
//SALIDAS:          unsigned char* con la imagen binarizada.
unsigned char *binarizar(unsigned char *img, int *ancho, int *alto, int umbral){
    /*Se prepara lo necesario para la imagen binarizada*/
    int canalesFiltro = 1;
    size_t tamanoFiltro = *ancho * *alto * canalesFiltro;
    int canalesBin = canalesFiltro;
    size_t tamanoBin = *ancho * *alto * canalesBin;

    /*Se reserva memoria para la imagen binarizada*/
    unsigned char *imgBin = malloc(tamanoBin);

    /*Se termina el programa en caso de error*/
    if(imgBin == NULL){
        printf("Error al reservar memoria para la imagen binarizada");
        exit(1);
    }

    /*Ciclo que recorre los pixeles de la imagen con filtro para binarizarla*/
    for(unsigned char *pf = img, *pb = imgBin; pf < img + tamanoFiltro; pf += canalesFiltro, pb += canalesBin){
        /*En caso de que el pixel sea mayor que el umbral se deja blanco*/
        if(*pf > umbral){
            *pb = 255;
        }

        /*En caso de que el pixel sea igual o menor que el umbral se deja negro*/
        else{
            *pb = 0;
        }
    }

    /*Se retorna la imagen binarizada*/
    return imgBin;
}

//ENTRADAS:         unsigned char* img con los pixeles de la imagen binarizada.
//                  Puntero a entero ancho con el ancho de la imagen.
//                  Puntero a entero alto con el alto de la imagen.
//                  int porcentaje con un valor del 0 al 100 que indica el porcentaje de pixeles negros en la imagen.
//
//FUNCIONAMIENTO:   Función que indica si una imagen es nearly black en caso de que el porcentaje de pixeles negros sea igual o
//                  mayor al indicado en porcentaje, entregando 1 en caso de se cumpla la condición y 0 en caso contrario.
//
//SALIDAS:          Entero que indica si una imagen es nearly black o no, siendo 0 en caso de que no lo sea y 1 en caso de que
//                  lo sea.
int isNearlyBlack(unsigned char *img, int *ancho, int *alto, int porcentaje){
    /*Se crea suma que contendrá la cantidad de pixeles negros*/
    float suma = 0;

    /*Se prepara lo necesario para recorrer los pixeles de la imagen*/
    int canalesBin = 1;
    size_t tamanoBin = *ancho * *alto * canalesBin;

    /*Ciclo que recorre la imagen pixel por pixel y guarda la cantidad de pixeles negros (0) encontrados*/
    for(unsigned char *pb = img; pb < img + tamanoBin; pb += canalesBin){
        if(*pb == 0){
            suma = suma + 1;
        }
    }

    /*Se calcula el porcentaje de pixeles negros*/
    float porcentajeNegro = 100 * (suma / (*ancho * *alto));

    /*Se retorna verdadero en caso de que el porcentaje real de pixeles negros sea igual o mayor al porcentaje indicado*/
    if(porcentajeNegro >= porcentaje){
        return 1;
    }

    /*Se retorna falso en caso de que el porcentaje real de pixeles negros sea menor al porcentaje indicado*/
    else{
        return 0;
    }
}

//ENTRADAS:         char nombreImg con el nombre con el que se guardará la imagen.
//                  int ancho con el ancho de la imagen a guardar.
//                  int alto con el alto de la imagen a guardar.
//                  unsigned char* imagen en con los pixeles de la imagen en blanco y negro (de un solo canal) a guardar.
//
//FUNCIONAMIENTO:   Función que escribe en memoria la imagen en blanco y negro indicada bajo el nombre indicado.
//
//SALIDAS:          Al ser una función void no tiene salida salvo por la imagen que se escribe en memoria.
void escribirImgBN(char* nombreImg, int ancho, int alto, unsigned char *imagen){
    /*Escribe una imagen de un solo canal con los parámetros recibidos y sin perder calidad*/
    stbi_write_jpg(nombreImg, ancho, alto, 1, imagen, 100);
}

int** imgCharAInt(unsigned char *img, int ancho, int alto, int canales){
    int i;
    int **matriz;
	matriz = (int **) malloc (canales*ancho*sizeof(int *));
    for (i = 0; i < canales*ancho; i++){
		matriz[i] = (int *) malloc (alto*sizeof(int));
	}

    int j;
    int valor;
    unsigned char *valorStr = img;
    for (j = 0; j < canales*ancho; j++){
        for (i = 0; i < alto; i++){
            valor = (int) *valorStr;
			matriz[j][i] = valor;
            valorStr++;
		}
	}
    return matriz;
}