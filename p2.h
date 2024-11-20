//
// Created by angela on 15/11/24.
//

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

//CONSTANTES DEFINIDAS


//FUNCIONES GENERALES P2
void deallocateGen();
void allocateGen();
void memfill();
void memdump();
void memoryGen();
void readfile();
void writefile();
void writeC();
void readC();
void recurse(int n);


//FUNCIONES "AUXILIARES" P2
void Do_pmap (void);
void Cmd_ReadFile (char *ar[]);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void do_DeallocateDelkey (char *args[]);
void do_AllocateMmap(char *arg[]);
void * MapearFichero (char * fichero, int protection);
void do_AllocateShared (char *tr[]);
void do_AllocateCreateshared (char *tr[]);
void * ObtenerMemoriaShmget (key_t clave, size_t tam);
void LlenarMemoria (void *p, size_t cont, unsigned char byte);
void ImprimirListaShared(tListM  * memList);
void ImprimirListaMmap(tListM *memList);

#endif //P2_H
