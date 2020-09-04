#ifndef FUNCIONES_H
#define FUNCIONES_H

void recibirDatos(int argc, char **argv, int *cantImg, int *umbralBin, int *porcentaje, char* *nombreMasc, int *bandera);
int *leerJPG(char* nombreImg, int *ancho, int *alto, int *canales);
int **leerFiltro(char* nombreArchivo);
int *convertirBN(int *img, int ancho, int alto, int canales);
int *aplicarFiltro(int *img, int ancho, int alto, char* nombreFiltro);
int *binarizar(int *img, int ancho, int alto, int umbral);
int isNearlyBlack(int *img, int ancho, int alto, int porcentaje);
int escribirImagen(int *img, int alto, int ancho, char *nombreImg);
#endif
