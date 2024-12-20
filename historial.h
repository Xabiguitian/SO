

#ifndef HISTORIAL_H
#define HISTORIAL_H


//LIBRERÍAS IMPLEMENTADAS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



//DEFINICIÓN DE CONSTANTES
#define MAXIMUND 4096
#define MAXH 1024
#define NULLH -1

//DEFFINICIÓN DE TIPOS
typedef struct  {
    char *comando[MAXIMUND];
    int lastPos;
} tList;





//FUNCIONES DE HISTORIAL.C
bool insertItemH(char commd[],tList *L);
void createEmptyListH(tList *historial);
char *getItemH(int i, tList historial);
int last(tList historial);
void deleteListH(tList *historial);
int firstH(tList historial);
int previousH(int pHist, tList historial);
bool isEmptyListH(tList historial);
int nextH(int pHist, tList historial);




#endif //HISTORIAL_H
