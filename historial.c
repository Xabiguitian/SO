//
// Created by angela on 24/09/24.
//

#include "historial.h"


//FUNCIONES AUXILIARES PARA EL HISTORIAL

bool insertItemH(char commd[] , tList *historial){
  char *comdau=malloc(sizeof(char[MAXIMUN])); //Creacion de una variable auxiliar para evitar perder
  // la información del comando



    if(historial->lastPos>MAXIMUND){
        return false;

    }else{
        historial->lastPos++; //añadimos en la ultima posicion un "hueco" más en el array.
        historial->commd[historial->lastPos]=comdau;
        strcpy(historial->commd[historial->lastPos],commd);

            return true;
        }
        free(comdau); //PREGUNTAR SI HAY QUE HACER UN NULL DESPUÉS DESTE FREE!!
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

