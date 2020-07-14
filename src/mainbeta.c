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

  int cantImagenes = 1;
  int umbralBin = 0;
  int umbralClas = 0;
  char* nombreMasc = NULL;
  int cont = 1;
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
            fprintf(stderr, "Opcion -%c requiere un argumento.\n", optopt);
        }
        
        else if (isprint (optopt))
          fprintf(stderr, "Opcion desconocida `-%c'.\n", optopt);

        else
          fprintf(stderr, "Opcion con caracter desconocido `\\x%x'.\n", optopt);
        return 1;

    default:
        abort ();
    }
  }
  
  while (cont <= cantImagenes)
  {
      //Leer imagen .jpg
      unsigned char *img = stbi_load("imagenes_entrada/test1.jpg", &ancho, &alto, &canales, 0);
      if (img == NULL)
      {
          printf("Error al cargar la imagen");
          exit(1);
      }
      
      //Convertir imagen a BN
      size_t tamano = ancho * alto * canales;
      int canalesGris = canales == 4 ? 2 : 1;
      size_t tamanoGris = ancho * alto * canalesGris;

      unsigned char *imgGris = malloc(tamanoGris);
      if(imgGris == NULL){
          printf("Error al reservar memoria para la imagen en blanco y negro");
          exit(1);
      }
      
      for(unsigned char *p = img, *pg = imgGris; p < img + tamano; p += canales, pg += canalesGris){
          *pg = (uint8_t)((*p * 0.3) + (*(p + 1) * 0.59) + (*(p + 2) * 0.11));
          if(canales == 4) {
              *(pg + 1) = *(p + 3);
          }
      }

      //Guardar imagen BN
      stbi_write_jpg("imagenes_salida/test2_BN.jpg", ancho, alto, canalesGris, imgGris, 100);
      
      //Aplicar filtro realce a imagen BN
      int canalesFiltro = canalesGris;
      size_t tamanoFiltro = ancho * alto * canalesFiltro;

      unsigned char *imgFiltro = malloc(tamanoFiltro);
      int pgInt = 0;
      for(unsigned char *pg = imgGris, *pf = imgFiltro; pg < imgGris + tamanoGris; pf += canalesFiltro, pg += canalesGris, pgInt += canalesGris){
          
          if (pgInt < (ancho * canalesGris) || pgInt >= (ancho * (alto - 1) * canalesGris) || pgInt % (ancho * canalesGris) == 0 || (pgInt + 1) % (ancho * canalesGris) == 0){
              *pf = *pg;
          }
          else{
              *pf = (uint8_t)((*pg * -4) + (*(pg - ((ancho + 1) * canalesGris)) * 0) + (*(pg - ((ancho) * canalesGris)) * 1)
                    + (*(pg - ((ancho - 1) * canalesGris)) * 0) + (*(pg - canalesGris) * 1) + (*(pg + canalesGris) * 1) + (*(pg + ((ancho - 1) * canalesGris)) * 0)
                    + (*(pg + ((ancho) * canalesGris)) * 1) + (*(pg + ((ancho + 1) * canalesGris)) * 0));
              /**pf = (uint8_t)((*pg * -4) + (*(pg - 1) * 1) + (*(pg + 1) * 1)
                    + (*(pg - ancho - 1) * 0) + (*(pg - ancho) * 1) + (*(pg - ancho + 1) * 0)
                    + (*(pg + ancho - 1) * 0) + (*(pg + ancho) * 1) + (*(pg + ancho + 1) * 0));*/
          }
      }
      //Guardar imagen con filtro
      stbi_write_jpg("imagenes_salida/test2_Filtro.jpg", ancho, alto, canalesFiltro, imgFiltro, 100);

      //Binarizar imagen con filtro
      int umbral = 200;
      int canalesBin = canalesFiltro;
      size_t tamanoBin = ancho * alto * canalesBin;

      unsigned char *imgBin = malloc(tamanoBin);
      for(unsigned char *pf = imgFiltro, *pb = imgBin; pf < imgFiltro + tamanoFiltro; pf += canalesFiltro, pb += canalesBin)
      {
          if (*pf > umbral){
              *pb = 255;
          }
          else{
              *pb = 0;
          }
      }

      //Guardar imagen binarizada
      stbi_write_jpg("imagenes_salida/test2_Binarizacion.jpg", ancho, alto, canalesBin, imgBin, 100);

      //Clasificar imagen en nearly black
      int porcentaje = 80;
      float suma = 0;
      for(unsigned char *pb = imgBin; pb < imgBin + tamanoBin; pb += canalesBin){
          if(*pb == 0){
              suma = suma + 1;
          }
      }

      float porcentajeNegro = 100 * (suma / (ancho * alto));
      if(porcentajeNegro >= porcentaje){
          printf("La imagen es nearly black.\n");
      }
      else{
          printf("La imagen no es nearly black.\n");
      }
      //Guardar resultado
      cont++;
  }
}