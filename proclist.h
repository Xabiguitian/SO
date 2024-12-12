#ifndef PROCLIST_H
#define PROCLIST_H


#define MAXDATA 4096
#define MAXVAR 4096

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

typedef enum status {
    FINISHED, STOPPED, SIGNALED, ACTIVE, UNKNOWN
} status;

typedef struct dataProc {
    pid_t pid;
    status estado;
    char * user;
    char * cmd;
    char * date;
    int state;
    int end;
} dataProc;

typedef struct tListProc {
    dataProc itemP [MAXDATA];
    int lastPos;
} tListProc;

void createEmptyProcList(tListProc *Proc);
bool isEmptyProcList(tListProc Proc);
int firstProcList(tListProc Proc);
int lastProcList(tListProc Proc);
dataProc getItemProcList(int p, tListProc Proc);
void deleteItemProcList(int p, tListProc *Proc);
void deleteProcList(tListProc *Proc);
void updateItemProcList(dataProc item, int p, tListProc *Proc);
bool insertItemProcList(dataProc item, tListProc *Proc);

#endif //PROCLIST_H
