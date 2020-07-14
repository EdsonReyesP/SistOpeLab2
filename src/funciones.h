#ifndef FUNCIONES_H
#define FUNCIONES_H

void recibirDatos(int argc, char **argv, int *cantImg, int *umbralBin, int *porcentaje, char* *nombreMasc, int *bandera);
unsigned char *leerJPG(char* nombreImg, int *ancho, int *alto, int *canales);
int **leerFiltro(char* nombreArchivo);
unsigned char *convertirBN(unsigned char *img, int *ancho, int *alto, int *canales);
unsigned char *aplicarFiltro(unsigned char *img, int *ancho, int *alto, char* nombreFiltro);
unsigned char *binarizar(unsigned char *img, int *ancho, int *alto, int umbral);
int isNearlyBlack(unsigned char *img, int *ancho, int *alto, int porcentaje);
void escribirImgBN(char* nombreImg, int ancho, int alto, unsigned char *imagen);
#endif
