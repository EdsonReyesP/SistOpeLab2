#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "funciones.h"

#define READ 0
#define WRITE 1

//Programa que lee una imagen y crea un proceso al cual enviar el resultado.
int main (int argc, char **argv){
    //Almacenar los datos
    int i, umbral, porcentaje, bandera;
    char* filtro;

    recibirDatos(argc, argv, &i, &umbral, &porcentaje, &filtro, &bandera);

    int **imagen;
    read(STDIN_FILENO, imagen, 3*7*9*sizeof(int));
    printf("Padre: %d\n", imagen[0][0]);
}