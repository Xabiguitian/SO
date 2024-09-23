#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGHT 4096
#define MAX_HISTORY 4096

char *history[MAX_HISTORY];
int historyCount=0;

//INDICE DE FUNCIONES DE LOS COMANDOS
void imprimirPrompt();
void authors();
void pid();
void ppid();
void cd();
void date();
void open();
void dup();
void infosys();
void help();
void quit();
void historic();


void trocearCadena(char *cadena, char *trozos[])
{
    int count=1;
    if ((trozos[0] = strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[count] = strtok(NULL," \n\t"))!=NULL)
        count++;
    return count;
}


void readInput(char *input){

    fgets(input,sizeof(input), stdin);
}



void printHistoric(char *historic){
    int i=0;

    while (historic[i] != -1){
        printf("%s", &historic[i]);
        i++;
    }
}

void addHistoric (char *comando){
    if (historyCount < MAX_HISTORY){
        history[historyCount] = strdup(comando);
        historyCount++;
    }else{
        printf("Error: History Full\n");
    }
}




int getCommand(*comando){ //función para conseguir el identificador del comando
    if(strcmp(comando[0]='authors')==0)
        return 0;
    else if(strcmp(comando[0]=='pid')==0)
        return 1;
    else if(strcmp(comando[0]=='ppid')==0)
        return 2;
    else if(strcmp(comando[0]=='cd')==0)
        return 3;
    else if(strcmp(comando[0]=='date')==0)
        return 4;
    else if(strcmp(comando[0]=='open')==0)
        return 5;
    else if(strcmp(comando[0]=='close')==0)
        return 6;
    else if(strcmp(comando[0]=='dup')==0)
        return 7;
    else if(strcmp(comando[0]=='infosys')==0)
        return 8;
    else if(strcmp(comando[0]=='help')==0))
        return 9;
    else if(strcmp(comando[0]=='quit'))==0||strcmp(comando[0]='exit')=0||strcmp(comando[0]='bye')
        return 10;
    else if(strcmp(comando[0]=='historic')==0)
        return 11;

    else
        return -1;


    }



}



void processCommand(char command[], int * salir){
    char * trozos[3];

    trocearCadena(command, trozos);//reformar sacado de otra practica

    switch (trozos[0]) {
        case 0:
            authors();
            break;
        case 1:
            pid();
        case 2:
            ppid();
        case 3:
            cd();
        case 4:
            date();
        case 5:
            open();
        case 6:
            close;
        case 7:
            dup();
        case 8:
            infosys();
        case 9:
            help();
        case 10:
            off();
        case 11:
            Historic();

        default:
            printf("No se reconoce el comando");
    }

}


int main(){
    char comando[MAX_COMMAND_LENGHT];

    leerComando(comando);
    addHistoric(comando);

    processCommand(comando);


    return 0;
}



void imprimirPrompt(){
    printf("# ");
}

void authors(){
    printf("Ángela García Sánchez (angela.gsanchez@udc.es)\nXabier Guitián López (x.guitian@udc.es)\n");
}

void pid(){
    printf("PID: %d\n", getpid());
}

void ppid(){
    printf("PID: %d\n", getppid());
}

void cd(){

}

void date(){

}

void open(){

}

void dup(){

}

void infosys(){

}

void help(){

}

void quit(){

}

void historic(){
    createEmptyList( *L);

}
