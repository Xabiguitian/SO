//
// Created by angela on 22/10/24.
//

#ifndef P1_H
#define P1_H

//LIBRERÍAS IMPLEMENTADAS
#include "historial.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

//CONSTANTES DEFINIDAS
#define MAX 1024 //maximo de letras en una palabra
#define MAXPROGRAM 4096 //maximos de elementos de la lista estática
#define MAXIMUN 3

//COMANDOS P1
void makefile(char *trozos[]);
void makedir(char *trozos[]);
void printPermissions(struct stat fileStat);
void listFile(char *trozos[]);
void listDir(char *trozos[]);
void cwd();
void reclist (char *trozos[]);
void revlist (char *trozos[]);
void erase(char *trozos[]);
void delrec(char *path);


#endif //P1_H
