#include <stdio.h>
#include "p0.h"
#include "p1.h"
#include "file.h"
#include "historial.h"


void imprimirPrompt(){
        printf("# ");
    }

int TrocearCadena(char * cadena, char * trozos[]){
        int i=1;
        if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
            return 0;
        while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
            i++;
        return i;
    }


    void leerComando(char *command,char * trozos[]) {
        fgets(command, MAX, stdin);
        TrocearCadena(command, trozos);//trocea las palabras del comando en trozos
    }




    //FUNCIÓNES PARA PROCESAR LOS COMANDOS
    void processCommand(char *command, tList *historial, char * trozos[],int *fin,filelist *F ) {
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
                EliminarFicheros(F);
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
            }else{
                printf("No se reconoce el comando.\n");
            }

        }

    }






int main() {
        char command[MAX];
        char *trozos[MAXTROZOS];
        tList historial;
        filelist F;
        int fin=1;


        createEmptyListH(&historial);
        createEmptyListF(&F);

        añadirFicheros(0, "entrada estandar", O_RDWR, &F);
        añadirFicheros(1, "salida estandar", O_RDWR, &F);
        añadirFicheros(2, "error estandar", O_RDWR, &F);

       do {
            imprimirPrompt();
            leerComando(command,trozos);
            processCommand(command, &historial, trozos,&fin ,&F);


        }while(fin==1);
    }