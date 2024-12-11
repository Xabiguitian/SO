#include <stdio.h>
#include "p0.h"
#include "p1.h"
#include "file.h"
#include "historial.h"
#include "p2.h"
#include "p3.h"



void imprimirPrompt(){
        printf("# ");
    }




void leerComando(char *command,char * trozos[]) {
    fgets(command, MAX, stdin);
    TrocearCadena(command, trozos);//trocea las palabras del comando en trozos
}




//FUNCIÓNES PARA PROCESAR LOS COMANDOS
void processCommand(char *command,char *input, tList *historial, char * trozos[],int *fin,filelist *F, tListM * mL, tListProc *listProc ,char *environp [], char *env[] ) {
    if(trozos[0]!=NULL){
        insertItemH(command,historial);
        if(strcmp(trozos[0], "authors")==0) {
            authors(trozos);
        }else if(strcmp(trozos[0],"pid")==0) {
            pid();
        }else if(strcmp(trozos[0],"ppid")==0) {
            ppid();
        }else if(strcmp(trozos[0],"cd")==0) {
            cd(trozos);
        }else if(strcmp(trozos[0],"date")==0){
            cmdate(trozos);
        }else if(strcmp(trozos[0],"open")==0) {
            Cmd_open(trozos, F);
        }else if(strcmp(trozos[0], "dup")==0) {
            Cmd_dup (trozos,F);
        }else if(strcmp(trozos[0], "close")==0) {
            Cmd_close (trozos,F);
        }else if(strcmp(trozos[0],"infosys")==0) {
            infosys();
        }else if(strcmp(trozos[0],"help")==0) {
            help(historial, trozos);
        }else if(strcmp(trozos[0],"quit")==0||strcmp(trozos[0],"exit")==0||strcmp(trozos[0],"bye")==0) {
            deleteListH(historial);
            freeMemList(mL);
            deleteMemList(mL);
            EliminarFicheros(F);
            deleteProcList(listProc);
            *fin=0;
            off();
        }else if(strcmp(trozos[0],"historic")==0) {
            cmdhistoric(trozos,historial);
        }else if(strcmp(trozos[0],"makefile")==0) {
            makefile(trozos);
        }else if(strcmp(trozos[0],"makedir")==0) {
            makedir(trozos);
        }else if(strcmp(trozos[0],"cwd")==0) {
            cwd();
        }else if(strcmp(trozos[0],"listfile")==0) {
            listFile(trozos);
        }else if(strcmp(trozos[0],"listdir")==0) {
            listDir(trozos);
        }else if(strcmp(trozos[0],"reclist")==0) {
            reclist(trozos);
        }else if(strcmp(trozos[0],"revlist")==0) {
            revlist(trozos);
        }else if(strcmp(trozos[0],"erase")==0) {
            erase(trozos);
        }else if(strcmp(trozos[0],"delrec")==0) {
            if (trozos[1] != NULL){
                delrec(trozos[1]);
            }else{
                printf("Error: Se debe especificar un archivo o directorio para eliminar recursivamente\n");
            }
        }else if(strcmp(trozos[0],"erase")==0) {
            erase(trozos);
        }else if(strcmp(trozos[0], "recurse")==0){
            recurse(trozos);
        }else if(strcmp(trozos[0],"memory")==0) {
            memoryGen(trozos,mL);
        }else if(strcmp(trozos[0],"writefile")==0){
            writefile(trozos);
        }else if(strcmp(trozos[0],"readfile")==0){
            Cmd_ReadFile(trozos);
        }else if(strcmp(trozos[0],"write")==0){
            writeC(trozos);
        }else if(strcmp(trozos[0],"read")==0){
            readC(trozos);
        }else if(strcmp(trozos[0], "memfill")==0){
            memfill(trozos);
        }else if(strcmp(trozos[0],"memdump")==0){
            memdump(trozos);
        }else if(strcmp(trozos[0],"allocate")==0){
            allocate(trozos, mL);
        }else if(strcmp(trozos[0],"deallocate")==0){
            deallocate(trozos, mL);
        }else if(strcmp(trozos[0],"getuid")==0){
            getUid();
        }else if(strcmp(trozos[0],"setuid")==0) {
            cmd_setUid(trozos);
        }else if (strcmp(trozos[0],"fork")==0) {
            Cmd_fork(trozos,listProc);
        }else if (strcmp(trozos[0],"showvar")==0) {
            showvar(trozos,environp,env);

        }else if (strcmp(trozos[0],"subsvar")==0) {
            subsvar(trozos,environp,env);
        }else if(strcmp(trozos[0],"changevar")==0){
            changevar(trozos,environp,env);

        }else if(strcmp(trozos[0],"environ")==0) {
            Cenviron(trozos, environp, env);
        }else if(strcmp(trozos[0],"exec")==0) {
            execCmd(trozos,input,historial, mL, listProc, environp);

        }else if(strcmp(trozos[0],"execpri")==0){
            execpri(trozos,input,historial, mL, listProc, environp);
        }else if(strcmp(trozos[0],"back")==0){
            back(trozos, listProc);
        }else if(strcmp(trozos[0],"listjobs")==0){
            listjobs(trozos, listProc);
        }else if(strcmp(trozos[0],"deljobs")==0){
            deljobs(trozos, listProc);
        }else if(strcmp(trozos[0],"search")==0){
            //search(trozos, searchList);
        }else if(strcmp(trozos[0],"fg")==0){
            fg(trozos, listProc);
        }else{
            printf("No se reconoce el comando.\n");
        }
    }
}






int main(int argc, char *argv[], char *environp[]) {
    char command[MAX];
    char *trozos[MAXTROZOS];
    char input[MAX];
    tList historial;
    filelist F;
    tListM mL;
    tListProc listProc;
    int fin=1;


    createEmptyListH(&historial);
    createEmptyListF(&F);
    createEmptyMemList(&mL);
    createEmptyProcList(&listProc);

    añadirFicheros(0, "entrada estandar", O_RDWR, &F);
    añadirFicheros(1, "salida estandar", O_RDWR, &F);
    añadirFicheros(2, "error estandar", O_RDWR, &F);

    do {
        imprimirPrompt();
        leerComando(command,trozos);
        processCommand(command, input,&historial, trozos,&fin ,&F,&mL,&listProc,environp, environ);
    }while(fin==1);
    printf("\n");
}
