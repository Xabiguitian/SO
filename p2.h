

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
void printMemoryList(tListM mL);
void aux_malloc(char *trozos[], tListM *mL);
void allocate(char *tr[], tListM L);
void Recursiva(int n);
void recurse(char *trozos[]);
void pMap(void);
void memoryGen(char *trozos[], tListM *mL);
void writefile(char *trozos[]);
void writeC(char *trozos[]);
void *cadtop(char *s);
ssize_t LeerFichero(char *f, void *p, size_t cont);
void Cmd_ReadFile(char *trozos[]);
void readC(char *trozos[]);
void LlenarMemoria(void *p, size_t cont, unsigned char byte);
void memfill(char *trozos[]);
void dumpMem(const char *p, int nbytes);
void memdump(char *trozos[]);
void deallocate(char *tr[], tListM *L);
void InsertarNodoShared(tListM *memList, void *dir, size_t tam, key_t clave);
void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListM L);
void do_AllocateCreateshared(char *tr[], tListM L);
void ImprimirListaShared(tListM *memList);
void do_AllocateShared (char *tr[], tListM L);
void do_DeallocateMalloc(size_t size, tListM L);
void do_DeallocateMmap(char *file, tListM L);
void deallocate_addr(void *addr, tListM L);
void do_DeallocateShared(char *key_str);
void do_DeallocateDelkey(char *tr[]);


#endif //P2_H
