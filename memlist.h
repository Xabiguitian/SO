

#ifndef MEMLIST_H
#define MEMLIST_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include "file.h"


#define MAXDATA 4096
#define MAX_LISTMEM 2048
#define MAX_FILENAME 1024


typedef struct fich {
    int df;
    char filename[MAX_FILENAME];
} fich;


typedef enum cmd {
    MALLOC, MMAP, SHARED
} cmd;


typedef struct dataMem {
    size_t size;        // Tamaño del bloque de memoria
    void *dir;          // Dirección del bloque de memoria
    char date[128];     // Fecha o información adicional
    union {
        int key;        // Clave para memoria compartida
        fich fichero;   // Información de archivo
    } Union;            // Unión para clave o archivo
    cmd cmdType;        // Tipo de comando (malloc, mmap, etc.)
} dataMem;


typedef struct  {
    dataMem itemM[MAX_LISTMEM];
    int lastPos;
} tListM;

tListM *L = NULL;

void createEmptyMemList(tListM *memList);
bool isEmptyMemList(tListM memList);
int nextMemListPos(int pMem, tListM memList);
int prevMemListPos(int pMem, tListM memList);
int firstMemListPos( tListM memList);
int lastMemListPos(tListM memList);
bool insertMemListPos(tListM* memList, dataMem m);
dataMem getDataItemList(tListM memList, int pMem) ;
void freeMemList(tListM *memList);
void deleteItemMemList( int pMem,tListM* memList);
void deleteMemList(tListM* memList);
void addMemBlock(void *ptr, size_t size, const char *type, tListM *mL);
void LlenarMemoria(void *ptr, size_t tam, int valor);
void *AsignarMemoriaMalloc(size_t tam);
void *AsignarMemoriaMmap(char *fichero, int protection);
void *AsignarMemoriaShared(key_t cl, size_t tam);
void InsertarBloqueMemoria(dataMem nuevoBloque);

#endif //MEMLIST_H
