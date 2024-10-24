//
// Created by angela on 24/10/24.
//
//
// Created by angela on 22/10/24.
//
//
// Created by angela on 24/09/24.
//
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


//FUNCIONES AUXILIARES PARA EL HISTORIAL

void createEmptyListF(filelist *F){
    F->lastPos=NULLF;
}


char * getItemF(int i, filelist F){
    if(i>=0&&i<=F.lastPos){
        return F.files[i].name;
    }else{
      return NULL;
	}
}

int lastF(filelist F){
    return F.lastPos;
}
int firstF(filelist F){
    return 0;
}
int previousF(int pFile, filelist F){
    return --pFile;
}
bool isEmptyListF(filelist F){
    return F.lastPos==NULLF;
}
bool insertItemF(char commd[] , filelist *F){

    if(F->lastPos>=MAXF-1){
        return false;
    }else{

        F->lastPos++;
         F->files[F->lastPos].id=F->lastPos;

        F->files[F->lastPos].name = malloc((strlen(commd)+1)*sizeof(char));
        if(F->files[F->lastPos].name==NULL){
          return false;
        }
        strcpy(F->files[F->lastPos].name,commd);

          F->files[F->lastPos].mode=0;

        return true;
        free( F->files[F->lastPos].name);
    }

}
int nextF(int pFile, filelist F){
    if(pFile==F.lastPos){
        return -1;
    }else{
        return ++pFile;
    }
}
//FUNCIONES AUXILIARES PARA LOS FICHEROS

void listarFicheros(filelist * F){
    int i;
    if(isEmptyListF(*F)){
        printf("No hay archivos en la lista.\n");
    }
    int pFile=firstF(*F);
    for(i=0;i<=lastF(*F);i++){
        printf("descriptor: %d -> %s ",  F->files[i].id, F->files[i].name);

        if(F->files[i].mode != -1) {
            if(F->files[i].mode & O_CREAT)
                printf("O_CREAT\n");
            else if(F->files[i].mode & O_EXCL)
                printf("O_EXCL\n");
            else if(F->files[i].mode & O_WRONLY)
                printf("O_WRONLY\n");
            else if(F->files[i].mode & O_RDWR)
                printf("O_RDWR\n");
            else if(F->files[i].mode & O_APPEND)
                printf("O_APPEND\n");
            else if(F->files[i].mode & O_TRUNC)
                printf("O_TRUNC\n");
            else //if (modo & O_RDONLY)
                printf("O_RDONLY\n");
        } else {
            continue;
        }

        pFile=nextF(pFile,*F);
    }
}


bool añadirFicheros(int id, char *name, int mode, filelist * F){
  //int atributos=fcntl(id, F_GETFL);
    if(F->lastPos>=MAXF-1){
      perror("No hay espacio para añadir el fichero\n");
      return false;
    }else{
      F->lastPos++;
      F->files[F->lastPos].id=id;

      if(mode == O_CREAT)
            F->files[F->lastPos].mode = O_CREAT;
        else if(mode == O_EXCL)
            F->files[F->lastPos].mode = O_EXCL;
        else if(mode == O_TRUNC)
            F->files[F->lastPos].mode = O_TRUNC;
        else
            F->files[F->lastPos].mode = fcntl(F->files[F->lastPos].id, F_SETFL, mode);

      F->files[F->lastPos].name = malloc(strlen(name)+1);
      strcpy(F->files[F->lastPos].name,name);
      return true;
    }
}




void EliminarFicheros(filelist *F){
    int i;
    if(isEmptyListF(*F)){
        perror("No hay ficheros que eliminar, la lista ya está vacía.\n");
    }else{
        for(i=0;i<=lastF(*F);i++){
            if(F->files[i].name!=NULL){
                free(F->files[i].name);
                F->files[i].name=NULL;
            }
        }
    }
    F->lastPos=-1;
}

void EliminarFichero(filelist *F, int df){
    int i, j;

    if(isEmptyListF(*F)){
        perror("No hay ficheros que eliminar, la lista ya está vacía.\n");
    }else{

        for(i=0;F->files[i].id!=df;i++);
        free(F->files[i].name);

        for(j = i; j < MAXF; j++) {
            F->files[j] = F->files[j + 1];
        }

        F->lastPos--;
    }
}