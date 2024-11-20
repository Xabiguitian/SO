

#ifndef P2_H
#define P2_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "memlist.h"

//CONSTANTES DEFINIDAS


//FUNCIONES GENERALES P2
//void deallocateGen();
//void allocateGen();
//void memfill();
//void memdump();
void memoryGen();
//void readfile(char *trozos[]);
void writefile(char *trozos[]);
void writeC(char *trozos[]);
//void readC(char *trozos[]);
void recurse(char *trozos[]);
void Cmd_ReadFile (char *trozos[]);
void readC(char *trozos[]);


#endif //P2_H
