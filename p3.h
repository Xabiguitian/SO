#ifndef P3_H
#define P3_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <errno.h>
#include "p0.h"
#include "p1.h"
#include "p2.h"
#include "memlist.h"
#include "file.h"
#include "proclist.h"
#include "searchdirlist.h"



extern char ** environ;



//COMANDOS DE LA PRÁCTICA 3
void getUid();
void cmd_setUid(char *trozos[]);
void Cmd_fork (char * tr[], tListProc * ListProc);
void subsvar(char *trozos[], char *arg3[], char *env[]);
void showvar(char * trozos[], char *arg3[], char * env[]);
void changevar(char *tr[], char * arg3[], char * env[]);
void printVar(char *env[], char * name);
void Cenviron(char *trozos[], char * arg3[], char *env[]);
void exec(char *trozos[], tSearchList *LibroDeBusqueda);
void execpri(char *trozos[], tSearchList *LibroDeBusqueda);
void back(char *trozos[], tListProc *listProc, tSearchList *LibroDeBusqueda);
void listar(tListProc *listProc);
void listjobs (char *trozos[], tListProc *list_proc);
dataProc actualizar_estado(dataProc item, int opciones);
void deljobs(char *trozos[], tListProc *listProc);
void search(char *trozos[], tSearchList *searchList);
void fg(char *trozos[], tListProc *listProc, tSearchList *LibroDeBusqueda);
void fgpri(char *trozos[], tListProc *listProc, tSearchList *LibroDeBusqueda);
void backpri(char *trozos[], tListProc *listProc, tSearchList *LibroDeBusqueda);
int cambiarPrioridad(char *val, pid_t pid);

#endif //P3_H