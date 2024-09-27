#include "historial.h"


//FUNCIONES AUXILIARES PARA EL HISTORIAL

bool insertItemH(char commd[] , tList *historial){

    if(historial->lastPos>MAXIMUND){
        return false;
    }else{
        //char * comdau = malloc(sizeof(char)*MAXIMUND);
        historial->lastPos++; //añadimos en la última posicion un "hueco" más en el array.
        historial->commd[historial->lastPos] = malloc(sizeof(char));
        strcpy(historial->commd[historial->lastPos],commd);
        return true;

        //free(comdau);
    }
}

void createEmptyListH(tList *historial){
    historial->lastPos=NULLH;
}


char * getItemH(int i, tList historial){

    if(i>=0&&i<MAXIMUN){
        return historial.commd[i];
    }else{
        return NULL;
    }
}

int last(tList historial){
    return historial.lastPos;
}

