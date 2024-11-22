//
// Created by angela on 16/11/24.
//
#include "memlist.h"

void createEmptyMemList(tListM* memList) {
    memList->lastPos=NULLM;

}

bool isEmptyMemList(tListM memList) {
    return memList.lastPos==NULLM;
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
    if(memList->lastPos<MAXDATA){
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
            if(shmget(item.key,0,0)!=-1){
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

void InsertarNodoMmap(tListM *lista, void *p, size_t tam, const char *filename, int df) {
    if (lista->lastPos >= MAX_LISTMEM) {
        printf("No hay espacio para más bloques de memoria.\n");
        return;
    }

    // Obtener la fecha y hora actual
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(lista->itemM[lista->lastPos].date, 128, "%Y-%m-%d %H:%M:%S", tm_info);

    // Llenar la entrada de la lista
    lista->itemM[lista->lastPos].size = tam;
    lista->itemM[lista->lastPos].dir = p;
    lista->itemM[lista->lastPos].cmdType = MMAP;
    lista->itemM[lista->lastPos].fichero.df = df;
    strncpy(lista->itemM[lista->lastPos].fichero.filename, filename, MAX_FILENAME);

    lista->lastPos++;
}