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
    if (p < 0 || p > Proc.lastPos) {
        fprintf(stderr, "Error: Índice fuera de rango en getItemProcList.\n");
        exit(EXIT_FAILURE); // Manejo de error
    }
    return Proc.itemP[p];
}


void deleteItemProcList(int p, tListProc *Proc) {
    // Liberar memoria asociada a las propiedades dinámicas
    if (Proc->itemP[p].user != NULL) {
        free(Proc->itemP[p].user);
    }
    if (Proc->itemP[p].cmd != NULL) {
        free(Proc->itemP[p].cmd);
    }
    if (Proc->itemP[p].date != NULL) {
        free(Proc->itemP[p].date);
    }

    // Reorganizar la lista
    for (int i = p; i < lastProcList(*Proc); i++) {
        Proc->itemP[i] = Proc->itemP[i + 1];
    }

    Proc->lastPos--;
}


void deleteProcList(tListProc *Proc) {
    while (!isEmptyProcList(*Proc)) {
        deleteItemProcList(firstProcList(*Proc), Proc);
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
    if (Proc->lastPos < MAXDATA - 1) { // Asegurar espacio disponible
        Proc->lastPos++;
        Proc->itemP[Proc->lastPos] = item;
        return true;
    } else {
        return false;
    }
}