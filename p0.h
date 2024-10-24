//
// Created by angela on 22/10/24.
//

#ifndef P0_H
#define P0_H

#include "historial.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/utsname.h>


//CONSTANTES DEFINIDAS
#define MAX 1024 //maximo de letras en una palabra
#define MAXPROGRAM 4096 //maximos de elementos de la lista estática
#define MAXTROZOS 100 //maximo de palabras que puede formar un comando de la shell

// COMANDOS P0
void authors(char *trozos[]);
void pid();
void ppid();
void cd(char * trozos[]);
void cmdate(char *trozos[]);
//void cmdopen();
//void cmddup();
void infosys();
void off();
//void cmdhistoric(char *trozos[], tList * historial);
//void cmdclose(char *trozos[]);
void help(tList *historial, char * trozos[]);


#endif //P0_H
