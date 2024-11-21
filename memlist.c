//
// Created by angela on 16/11/24.
//
#include "memlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  // Incluye la definición de struct stat
#include <fcntl.h>     // Incluye la definición de O_RDONLY, O_RDWR
#include <sys/mman.h>  // Incluye la definición de mmap
#include <unistd.h> 

tListM M;

void createEmptyMemList(tListM* memList) {
    memList->lastPos=-1;

}

bool isEmptyMemList(tListM memList) {
    return memList.lastPos==-1;
}

int nextMemListPos(int pMem, tListM memList) {
    if(pMem==memList.lastPos)
        return -1;
    else
        return ++pMem;
}

int prevMemListPos(int pMem, tListM memList) {
    return --pMem;
}

int firstMemListPos( tListM memList) {
    return 0;
}

int lastMemListPos(tListM memList) {
    return memList.lastPos;
}


bool insertMemListPos(tListM* memList, dataMem m) {
    if(memList->lastPos<MAX_LISTMEM){
        memList->lastPos++;
        memList->itemM[memList->lastPos]=m;
        return true;
    }else
        return false;

}


dataMem getDataItemList(tListM memList, int pMem) {
    return memList.itemM[pMem];

}


void freeMemList(tListM *memList) {
    int pos;
    dataMem item;

    for(pos=firstMemListPos(*memList);pos<=lastMemListPos(*memList);pos++){
        item=getDataItemList(*memList,pos);

        if(item.cmdType==MALLOC)
            free(item.dir);
        else if(item.cmdType==SHARED){
            if(shmget(item.Union.key,0,0)!=-1){
                if(shmdt(item.dir)==-1)
                    return;
            }
        }else if(item.cmdType==MMAP)
            munmap(item.dir, item.size);
    }

}

void deleteItemMemList( int pMem,tListM *memList) {
    int i;
    for(i=0; i<lastMemListPos(*memList);i++){
        if(i>=pMem)
            memList->itemM[i]=memList->itemM[i+1];
    }
    memList->lastPos--;
}

void deleteMemList(tListM* memList) {
    int i;
    for(i=0;i<=memList->lastPos;i--){
        deleteItemMemList(i,memList);
        memList->lastPos--;
    }
}

void LlenarMemoria(void *ptr, size_t tam, int valor) {
    if (ptr != NULL) {
        memset(ptr, valor, tam);  // Rellena la memoria con el valor proporcionado
    }
}

void *AsignarMemoriaMalloc(size_t tam) {
    void *ptr = malloc(tam);
    if (ptr != NULL) {
        LlenarMemoria(ptr, tam, 0);  // Rellenamos con 0
    }
    return ptr;
}

void *AsignarMemoriaMmap(char *fichero, int protection) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;

    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;

    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;

    return p;
}

void *AsignarMemoriaShared(key_t cl, size_t tam) {
    void *p;
    int id, flags = 0777;

    if (tam) {
        flags = flags | IPC_CREAT | IPC_EXCL;
    }

    if (cl == IPC_PRIVATE) {
        errno = EINVAL;
        return NULL;
    }

    if ((id = shmget(cl, tam, flags)) == -1)
        return NULL;

    if ((p = shmat(id, NULL, 0)) == (void *)-1) {
        int aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return NULL;
    }

    return p;
}

void InsertarBloqueMemoria(dataMem nuevoBloque) {
    if (M.lastPos < MAX_LISTMEM) {
        M.itemM[M.lastPos++] = nuevoBloque;
    } else {
        printf("Lista de memoria llena\n");
    }
}