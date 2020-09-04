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
    int i;

    i = atoi(argv[0]);

    //Leer imagen
    /*Se crean los datos necesarios para cada iteración*/
    int ancho, alto, canales;
    char* nombre = malloc(32);
    sprintf(nombre, "imagenes_entrada/imagen_%d.jpg", i);

    /*Se lee la imagen_i*/
    int *img = leerJPG(nombre, &ancho, &alto, &canales);

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

        char* altoStr = malloc(5);
        char* anchoStr = malloc(5);
        char *canalesStr = malloc(5);
        sprintf(altoStr, "%d", alto);
        sprintf(anchoStr, "%d", ancho);
        sprintf(canalesStr, "%d", canales);

        //Ejecuta BlancoNegro como proceso aparte pasando los parámetros en arcv
        char *argumentos[] = {argv[0], argv[1], argv[2], argv[3], argv[4], altoStr, anchoStr, canalesStr, NULL};
        execv("./gray_process", argumentos);
    }

    //Enviar imagen leída por pipe
    else{
        /*Se convierte la imagen de int* a int array*/
        int c;
        int tamano = ancho * alto * canales;
        int imagen[tamano];
        for(c = 0; c < tamano; c++){
            imagen[c] = img[c];
        }

        /*Se envía la imagen leida al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para continuar con la siguiente iteración del for*/
        waitpid(pid, &status, 0);
    }

    return 0;
}