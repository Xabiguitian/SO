

#ifndef P0_H
#define P0_H

#include "historial.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/utsname.h>
#include <errno.h>
#include "p1.h"
#include "file.h"


//CONSTANTES DEFINIDAS
#define MAX 1024 //maximo de letras en una palabra
#define MAXPROGRAM 4096 //maximos de elementos de la lista estática
#define MAXTROZOS 100 //maximo de palabras que puede formar un comando de la shell
#define MAXNAME 100



// COMANDOS P0
void authors(char *trozos[]);
void pid();
void ppid();
void cd(char * trozos[]);
void cmdate(char *trozos[]);
void Cmd_open(char * tr[], filelist *F);
void Cmd_dup (char * tr[],filelist *F);
void infosys();
void off();
void cmdhistoric(char *trozos[], tList * historial);
void Cmd_close (char *tr[],filelist *F);
void help(tList *historial, char * trozos[]);



#endif //P0_H
