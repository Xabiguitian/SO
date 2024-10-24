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

void deleteListF(filelist *F){ //funcion para borrar el historial antes de cerrar el terminal
    int i;
    for(i=lastF(*F);i>=0;i--){
        free(F->fileName[i]);
    }
}



void createEmptyListF(filelist *F){
    F->lastPos=NULLF;
}


char * getItemF(int i, filelist F){

    if(i>=0&&i<=F.lastPos){
        return F.fileName[i];
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

    if(F->lastPos>=MAXIMUND-1){
        return false;
    }else{
        //char * comdau = malloc(sizeof(char)*MAXIMUND);
        F->lastPos++; //añadimos en la ultima posicion un "hueco" más en el array.
        F->fileName[F->lastPos] = malloc((strlen(commd)+1)*sizeof(char));
        strcpy(F->fileName[F->lastPos],commd);

        return true;
        free(F->fileName);
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
        printf("%s\n",  getItemF(i,*F));
        pFile=nextF(pFile,*F);
    }
}


void añadirFicheros(int id, char *name, int mode, filelist * F){
    int i;
    int atributos=fcntl(id,F_GETFL);
    long lastFile=sysconf( _SC_OPEN_MAX );

    for(i=0;i<lastFile&&F->fileName[i]!=NULL;i++){
        if(i<lastFile){
            tFile newFile;;
            newFile.id=id;
            strcpy(newFile.name,name);
            newFile.mode=mode;
            if(atributos!=-1){
                if(atributos & O_EXCL){
                    strcpy(newFile.opening, "O_EXCL");
                }else if(atributos & O_CREAT){
                    strcpy(newFile.opening, "O_CREAT");
                }else if(atributos & O_TRUNC){
                    strcpy(newFile.opening, "O_TRUNC");
                }else if(atributos & O_APPEND){
                    strcpy(newFile.opening, "O_APPEND");
                }else if(atributos & O_WRONLY){
                    strcpy(newFile.opening, "O_WRONLY");
                }else if(atributos & O_RDWR){
                    strcpy(newFile.opening, "O_RDWR");
                }else if(atributos & O_RDONLY){
                    strcpy(newFile.opening, "O_RDONLY");
                }
            }
            F->fileName[i]=strdup(name);
            F->info=&newFile;
            F->lastPos=i;
        }else{
            perror("Error al añadir fichero, no hay espacio en la lista.\n");
        }
    }

}

void EliminarFicheros(filelist *F){
    int i;
    if(isEmptyListF(*F)){
        perror("No hay ficheros que eliminar, la lista ya está vacía.\n");
    }else{
        for(i=0;i<=lastF(*F);i++){
            if(F->fileName[i]!=NULL){
                printf("%s\n",F->fileName[i]);
                free(F->fileName[i]);
                F->fileName[i]=NULL;
            }
        }
    }
}
