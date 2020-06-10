#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main (int argc, char **argv) {
  int ancho, alto, canales;
  unsigned char *img = stbi_load("imagenes_entrada/test1.jpg", &ancho, &alto, &canales, 0);
  if(img == NULL){
      printf("Error al cargar la imagen");
      exit(1);
  }
  printf("Alto:%d\nAncho:%d\nCanales:%d", alto, ancho, canales);

  /*int cantImagenes = 0;
  int umbralBin = 0;
  int umbralClas = 0;
  char* nombreMasc = NULL;
  int index;
  int c;
  opterr = 0;

  while ((c = getopt (argc, argv, "c:u:n:m:")) != -1){
    switch (c)
    {
    case 'c':
        sscanf(optarg, "%d", &cantImagenes);
        break;
    
    case 'u':
        sscanf(optarg, "%d", &umbralBin);
        break;

    case 'n':
        sscanf(optarg, "%d", &umbralClas);
        break;

    case 'm':
        nombreMasc = optarg;
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
            fprintf(stderr, "Opcion -m requiere un argumento.\n");
        }
        
        else if (isprint (optopt))
          fprintf(stderr, "Opcion desconocida `-%c'.\n", optopt);

        else
          fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);
        return 1;

    default:
        abort ();
    }
  }*/
}