//
// Created by angela on 22/10/24.
//
//
// Created by angela on 24/09/24.
//

#include "historial.h"


//FUNCIONES AUXILIARES PARA EL HISTORIAL

void deleteListH(tList *historial){ //funcion para borrar el historial antes de cerrar el terminal
    int i;
    for(i=last(*historial);i>=0;i--){
        free(historial->comando[i]);
    }
}

bool insertItemH(char commd[] , tList *historial){

    if(historial->lastPos>=MAXIMUND-1){
        return false;
    }else{
        //char * comdau = malloc(sizeof(char)*MAXIMUND);
        historial->lastPos++; //añadimos en la ultima posicion un "hueco" más en el array.
        historial->comando[historial->lastPos] = malloc((strlen(commd)+1)*sizeof(char));
        strcpy(historial->comando[historial->lastPos],commd);
        return true;
        free(historial->comando);
    }

}

void createEmptyListH(tList *historial){
    historial->lastPos=NULLH;
}


char * getItemH(int i, tList historial){

    if(i>=0&&i<MAXIMUND){
        return historial.comando[i];
    }else{
        return NULL;
    }
}

int last(tList historial){
    return historial.lastPos;
}
int firstH(tList historial){
    return 0;
}
int previousH(int pHist, tList historial){
    return --pHist;
}
bool isEmptyListH(tList historial){
    return historial.lastPos==NULLH;
}

/*int nextH(int pHist, tList historial){
  int i;
  if(i<=historial.lastPos){
    return i+1;
    }else{
      return NULLH;
}
}*/
