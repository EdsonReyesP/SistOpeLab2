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
    /*Se extrae i de argv para saber que imagen leer*/
    int i;
    i = atoi(argv[0]);

    /*Se crean los datos necesarios para leer la imagen y almacenar la información importante*/
    int ancho, alto, canales;
    char* nombre = malloc(32);
    sprintf(nombre, "imagenes_entrada/imagen_%d.jpg", i);

    /*Se lee la imagen_i.jpg*/
    int *img = leerJPG(nombre, &ancho, &alto, &canales);

    /*Se crea variable para la espera del hijo*/
    int status;

    /*Se crea el pipe por el cual se enviará la imagen*/
    int pipeImg[2];
    pipe(pipeImg);

    /*Se crea un proceso hijo*/
    pid_t pid = fork();

    if(pid == 0){   //Proceso Hijo
        /*Se cierra la escritura del pipe y se hace dup2 para permitir la comunicación*/
        close(pipeImg[WRITE]);
        dup2(pipeImg[READ], STDIN_FILENO);

        /*Se convierte la información extraida de la imagen para ser enviados como argumentos al siguiente proceso*/
        char* altoStr = malloc(5);
        char* anchoStr = malloc(5);
        char *canalesStr = malloc(5);
        sprintf(altoStr, "%d", alto);
        sprintf(anchoStr, "%d", ancho);
        sprintf(canalesStr, "%d", canales);

        /*Se ejecuta gray_process como proceso aparte pasando los parámetros en arcv*/
        char *argumentos[] = {argv[0], argv[1], argv[2], argv[3], argv[4], altoStr, anchoStr, canalesStr, NULL};
        execv("./gray_process", argumentos);
    }

    else{   //Proceso Padre
        /*Se convierte la imagen de int* a int array para poder enviarla por el pipe*/
        int c;
        int tamano = ancho * alto * canales;
        int imagen[tamano];
        for(c = 0; c < tamano; c++){
            imagen[c] = img[c];
        }

        /*Se envía la imagen leida al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para avisar a main cuando se esté listo*/
        waitpid(pid, &status, 0);
    }

    return 0;
}