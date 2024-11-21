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
//void printMemoryList(tListM mL);
//void pMap(void);
//void memoryGen(char *trozos[], tListM mL);
//void writefile(char *trozos[]);
//void writeC(char *trozos[]);
//void * cadtop(char *s);
//ssize_t LeerFichero (char *f, void *p, size_t cont);
//void Cmd_ReadFile (char *trozos[]);
//void readC(char *trozos[]);
//void fillMem(void *p, size_t size, unsigned char value);
//void memfill(char *trozos[]);
void aux_malloc(char *trozos[], tListM *mL);
void allocate(char *tr[]);
void deallocate(char *tr[]);
void InsertarNodoShared(tListM *memList, void *dir, size_t tam, key_t clave);
void *ObtenerMemoriaShmget(key_t clave, size_t tam);
void do_AllocateCreateshared (char *tr[]);
void ImprimirListaShared(tListM  * memList);
void do_AllocateShared (char *tr[]);
void * MapearFichero (char * fichero, int protection);
void ImprimirListaMmap(tListM *memList);
void do_AllocateMmap(char *arg[]);
//void do_DeallocateDelkey (char *args[]);
//ssize_t LeerFichero (char *f, void *p, size_t cont);
//void Cmd_ReadFile (char *ar[]);
//void Do_pmap (void);
//void do_DeallocateMalloc(size_t size);
//void do_DeallocateMmap(char *file);
//void do_Deallocate(char *trozos[]);
//void inicializa();
//void inicializaM();
//void inicializaHistorial();
//void memory_funcs();
//void memory_vars();
//void Cmd_memory(char *trozos[]);


#endif //P2_H
