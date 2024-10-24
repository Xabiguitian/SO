//
// Created by angela on 24/10/24.
//

#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <errno.h>

#define NULLF -1;
#define MAXIMUND 4096
#define MAXF 1024

//STRUCT FICHEROS
typedef struct{
    int id;
    char *name;
    int mode;
}tFile;

typedef struct{
    int lastPos;
    tFile files[MAXF];
}filelist;








//FUNCIONES DE HISTORIAL.C
bool insertItemF(char filename[],filelist *F);
void createEmptyListF(filelist *F);
char *getItemF(int i, filelist F);
int lastF(filelist F);
int firstF(filelist F);
int previousF(int pFile, filelist F);
bool isEmptyListF(filelist F);
int nextF(int pFile, filelist F);
void EliminarFicheros(filelist *F);
bool añadirFicheros(int id, char *name, int mode, filelist * F);
void listarFicheros(filelist * F);
void EliminarFichero(filelist *F, int df);

#endif //FILE_H
