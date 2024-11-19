//
// Created by angela on 16/11/24.
//

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



typedef enum cmd {
    MALLOC, MMAP, SHARED
} cmd;


typedef struct dataMem {
    size_t size;
    void * dir;
    char date[128];
    union {
        int key;
        tFile fichero;
    } Union;
    cmd cmdType;
} dataMem;


typedef struct  {
    dataMem itemM[MAX_LISTMEM];
    int lastPos;
} tListM;

void createEmptyMem(tListM *memList);
bool isEmptyMemList(tListM memList);
int nextMem(int p, tListM memList);
int previousMem(int p, tListM memList);
int firstMem(tListM memList);
int lastMem(tListM memList);
bool insertMem(dataMem m, tListM *memList);
dataMem getDataMemList(int pMem, tListM memList);
void freeList(tListM *memList);
void deleteItemMemList(int pMem, tListM *M);
void deleteMem(tListM *memList);


#endif //MEMLIST_H
