#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "funciones.h"

//Programa que lee una imagen y crea un proceso al cual enviar el resultado.
int main (int argc, char **argv){
    int buffer[5];
    read(STDIN_FILENO, buffer, sizeof(buffer));
    printf("Padre: %s\n", buffer);
    return 0;
    //Almacenar los datos
    //Leer imagen
    //Crear pipe
    //Fork a Conversor en BN
    //Execv con datos sin la imagen
    //Enviar imagen leída por pipe
}