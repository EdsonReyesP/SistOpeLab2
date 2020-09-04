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
    int alto, ancho, canales, tamano;

    alto = atoi(argv[5]);
    ancho = atoi(argv[6]);
    canales = atoi(argv[7]);
    tamano = alto * ancho * canales;

    int imgBuffer[tamano];
    read(STDIN_FILENO, imgBuffer, tamano * sizeof(int));
    int *imgColor = malloc(tamano * sizeof(int));
    for(int c = 0; c < tamano; c++){
        imgColor[c] = imgBuffer[c];
    }

    int *imgBN = convertirBN(imgColor, ancho, alto, canales);

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
        
        //Ejecuta BlancoNegro como proceso aparte pasando los parámetros en argv
        char *argumentos[] = {argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL};
        execv("./filter", argumentos);
    }

    //Enviar imagen en blanco y negro por pipe
    else{
        /*Se convierte la imagen de int* a int array*/
        int tamanoBN = ancho * alto;
        int imagen[tamanoBN];
        for(int c = 0; c < tamanoBN; c++){
            imagen[c] = imgBN[c];
        }

        /*Se envía la imagen leida al hijo*/
        close(pipeImg[READ]);
        write(pipeImg[WRITE], imagen, sizeof(imagen));

        /*Se espera al hijo para continuar con la siguiente iteración del for*/
        waitpid(pid, &status, 0);
    }

    return 0;
}