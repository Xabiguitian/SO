#include <stdio.h>
#include "p0.h"
#include "p1.h"

//INDICE DE FUNCIONES DE LOS COMANDOS
void imprimirPrompt();
void leerComando(char *command);
void processCommandTrocearCadena(char command[],tList *historial, char * trozos[]);

//MAIN
int main()
{
    char command[MAXIMUN];
    char *trozos;
    tList historial;


    createEmptyListH(&historial);

    while(true) {
        imprimirPrompt();
        leerComando(command);
        processCommandTrocearCadena(command, &historial, &trozos);

    }
}




//IMPLEMENTACIÓN DE FUNCIONES
void imprimirPrompt(){
    printf("# ");
}

void leerComando(char *command) {
    fgets(command, MAX, stdin);
}




//FUNCIÓN PARA TROCEAR LA CADENA
int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}




//FUNCIÓNES PARA PROCESAR LOS COMANDOS
void processCommand(char *command, tList *historial, char * trozos[]) {
    command=trozos[0];

    if(strcmp(command, "authors")==0) {
        authors(trozos);
    }else if(strcmp(command,"pid")==0) {
        pid();
    }else if(strcmp(command,"ppid")==0) {
        ppid();
    }else if(strcmp(command,"cd")==0) {
        cd(trozos);
    }else if(strcmp(command,"date")==0){
        cmdate(trozos);
    }else if(strcmp(command,"open")==0) {
        //cmdopen();
    }else if(strcmp(command, "dup")==0) {
        //cmddup();
    }else if(strcmp(command, "close")==0) {
        //cmdclose(trozos);
    }else if(strcmp(command,"infosys")==0) {
        infosys();
    }else if(strcmp(command,"help")==0) {
        help(historial, trozos);
    }else if(strcmp(command,"quit")==0||strcmp(command,"exit")==0||strcmp(command,"bye")==0) {
        off();
    }else if(strcmp(command,"historic")==0) {
        //cmdhistoric(trozos,historial);
    }else if (strcmp(command,"makefile")==0){
        makefile(trozos);
    }else if (strcmp(command,"makedir")==0){
        makedir(trozos);
    }else if (strcmp(command,"listfile")==0){
        listFile(trozos);
    }else if (strcmp(command,"listdir")==0){
        listDir(trozos);
    }else if (strcmp(command,"cwd")==0){
        cwd();
    }else if (strcmp(command,"reclist")==0){
        reclist(trozos);
    }else if (strcmp(command,"revlist")==0){
        revlist(trozos);
    }else if (strcmp(command,"erase")==0){
        erase(trozos);
    }else if (strcmp(command,"delrec")==0){
        if (trozos[1] != NULL)
        {
            delrec(trozos[1]);
        }else{
            printf("Error: Se debe especificar un archivo o directorio para eliminar recursivamente\n");
        }
    }else
        printf("No se reconoce el comando.\n");
}



void processCommandTrocearCadena(char command[],tList *historial, char * trozos[]) {
    char *comdau=malloc(sizeof(*command)); //Creacion de una variable auxiliar para evitar perder
    // la información del comando
    strcpy(comdau,command);

    insertItemH(comdau,historial);
    TrocearCadena(comdau, trozos);
    processCommand(command,historial, trozos);


    free(comdau); //MIRAR SI AQUI HAY QUE HACER UN NULL DESPUÉS DEL FREE!!
}