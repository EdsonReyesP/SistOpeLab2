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

    //Leer imagen
    /*Se crean los datos necesarios para cada iteración*/
    int ancho, alto, canales;
    char* nombre = malloc(32);
    sprintf(nombre, "imagenes_entrada/imagen_%d.jpg", i);

    /*Se lee la imagen_i*/
    unsigned char *img = leerJPG(nombre, &ancho, &alto, &canales);

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

        char* iStr = malloc(4);
        char* umbralStr = malloc(10);
        char* porcentajeStr = malloc(10);
        char* banderaStr = malloc(2);
        sprintf(iStr, "%d", i);
        sprintf(umbralStr, "%d", umbral);
        sprintf(porcentajeStr, "%d", porcentaje);
        sprintf(banderaStr, "%d", bandera);

        //Ejecuta BlancoNegro como proceso aparte pasando los parámetros en arcv
        char *argumentos[] = { "-c", iStr, "-u", umbralStr, "-n", porcentajeStr, "-b", banderaStr, "-m", filtro, (const char*) NULL};
        execv("./BlancoNegro", argumentos);
    }

    //Enviar imagen leída por pipe
    else{
        /*Se pasan los pixeles de char a una matriz de int*/
        int **imagen = imgCharAInt(img, ancho, alto, canales);

        /*Se envía la imagen leida al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para continuar con la siguiente iteración del for*/
        waitpid(pid, &status, 0);
    }
}