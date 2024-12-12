#ifndef SEARCHDIRLIST_H
#define SEARCHDIRLIST_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAXDIRS 100  // Número máximo de directorios en la lista
#define MAXPATH 256  // Tamaño máximo de las rutas
#define NAMEMAX 4096

// Estructura para la lista de búsqueda
typedef struct tSearchList{
    char dirs[MAXDIRS][MAXPATH]; // Almacena los directorios
    int lastPosSearch;                  // Número de directorios en la lista
} tSearchList;

tSearchList LibroDeBusqueda;

void createEmptySearchList(tSearchList *searchList);
bool isEmptySearchList(tSearchList searchList);
int firstSearchList(tSearchList searchList);
int lastSearchList(tSearchList searchList);
int nextSearchList(tSearchList searchList,int pSearch);
bool insertSearchList(tSearchList *searchList, char *dir);
void removeSearchList(tSearchList *searchList, char *dir);
void removePositionSearchList(tSearchList *searchList, int position);
char *getItemSearchList(tSearchList searchList, int position);
void updateSearchList(tSearchList *searchList, char *dir);
char *Ejecutable(char *s);


#endif //SEARCHDIRLIST_H
