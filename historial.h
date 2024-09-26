//
// Created by angela on 24/09/24.
//

#ifndef HISTORIAL_H
#define HISTORIAL_H


//LIBRERÍAS IMPLEMENTADAS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



//DEFINICIÓN DE CONSTANTES
#define MAXIMUND 4096
#define MAXIMUN 1024
#define NULLH -1

//DEFFINICIÓN DE TIPOS
typedef struct  {
    char *commd[MAXIMUND];
    int lastPos;
} tList;





//FUNCIONES DE HISTORIAL.C
bool insertItemH(char commd[],tList *L);
void createEmptyListH(tList *historial);
char *getItemH(int i, tList historial);
int last(tList historial);





#endif //HISTORIAL_H
