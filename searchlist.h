//
// Created by angela on 3/12/24.
//

#ifndef SEARCHDIRLIST_H
#define SEARCHDIRLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAXDIRS 100  // Número máximo de directorios en la lista
#define MAXPATH 256  // Tamaño máximo de las rutas

// Estructura para la lista de búsqueda
typedef struct {
    char dirs[MAXDIRS][MAXPATH]; // Almacena los directorios
    int lastPosSearch;                  // Número de directorios en la lista
} tSearchList;

void createEmptySearchList(tSearchList *searchList);
bool isEmptySearchList(tSearchList searchList);
int firstSearchList(tSearchList searchList);
int lastSearchList(tSearchList searchList);
int nextSearchList(tSearchList searchList,int pSearch);
bool insertSearchList(tSearchList *searchList, char dir);
void removeSearchList(tSearchList *searchList, char dir);
void removePositionSearchList(tSearchList *searchList, int position);
tSearchList getItemSearchList(char dir);
void updateSearchList(tSearchList *searchList, char dir);


#endif //SEARCHDIRLIST_H

