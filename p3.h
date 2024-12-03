//
// Created by angela on 3/12/24.
//

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




//COMANDOS DE LA PRÁCTICA 3
void getUid();
void setUid(char *trozos[], char *id);
void Cfork(char * trozos[], tListProc * ListProc);

#endif //P3_H
