

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
#define MAX_FILENAME 256
#define NULLM -1

typedef struct fich {
    int df;
    char filename[MAX_FILENAME];
} fich;


typedef enum cmd {
    MALLOC,SHARED,MMAP
} cmd;


typedef struct dataMem {
    size_t size;
    void * dir;
    char date[128];
     int key;
    fich fichero;
    cmd cmdType;
} dataMem;


typedef struct  {
    dataMem itemM[MAX_LISTMEM];
    int lastPos;
} tListM;

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
void InsertarNodoMmap(tListM *lista, void *p, size_t tam, const char *filename, int df);


#endif //MEMLIST_H
