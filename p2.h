

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
void Recursiva (int n);
void recurse(char *trozos[]);
void printMemoryList(tListM mL);
void pMap(void);
void memoryGen(char *trozos[], tListM mL);
void writefile(char *trozos[]);
void writeC(char *trozos[]);
void * cadtop(char *s);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void Cmd_ReadFile (char *trozos[]);
void readC(char *trozos[]);
void fillMem(void *p, size_t size, unsigned char value);
void memfill(char *trozos[]);
void allocate(char *trozos[]);
void deallocate(char *tr[]);
//void deallocateGen();
//void allocateGen();
//void memdump();


#endif //P2_H
