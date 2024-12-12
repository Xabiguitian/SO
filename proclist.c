#include "proclist.h"

void createEmptyProcList(tListProc *Proc) {
    Proc->lastPos = -1;
}

bool isEmptyProcList(tListProc Proc) {
    return Proc.lastPos == -1;
}

int firstProcList(tListProc Proc) {
    return 0;
}

int lastProcList(tListProc Proc) {
    return Proc.lastPos;
}

dataProc getItemProcList(int p, tListProc Proc) {
    return Proc.itemP[p];
}

void deleteItemProcList(int p, tListProc *Proc) {
    int i;

    for(i = 0; i < lastProcList(*Proc); i++) {
        if(i >= p)
            Proc->itemP[i] = Proc->itemP[i+1];
    }

    Proc->lastPos--;
}

void deleteProcList(tListProc *Proc) {
    int i;

    for(i = 0; i <= Proc->lastPos; i++) {
        deleteItemProcList(i, Proc);
        Proc->lastPos--;
    }
}

void updateItemProcList(dataProc item, int p, tListProc *Proc) {
    int i;

    for(i = 0; i <= lastProcList(*Proc); i++) {
        if(i == p)
            Proc->itemP[i] = item;
    }
}

bool insertItemProcList(dataProc item, tListProc *Proc) {
    if(Proc->lastPos < MAXDATA) {
        Proc->lastPos++;
        Proc->itemP[Proc->lastPos] = item;
        return true;
    } else
        return false;
}
