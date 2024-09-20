#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_COMMAND_LENGHT 4096
#define MAX_HISTORY 4096

char *history[MAX_HISTORY];
int historyCount=0;

void trocearCadena(char *cadena, char *trozos[])
{
    int count=1;

    if ((trozos[0] = strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[count] = strtok(NULL," \n\t"))!=NULL)
        count++;
    return count;
}

void imprimirPrompt(){
    printf("# ");
}

void leerComando(char *comando) {
    imprimirPrompt();
    fgets(comando, MAX_COMMAND_LENGHT, stdin);
    comando[strcspn(comando, "\n")] = '\0'; // Eliminar el salto de línea
}

void addHistoric (char *comando){
    if (historyCount < MAX_HISTORY){
        history[historyCount] = strdup(comando);
        historyCount++;
    }else{
        printf("Error: History Full\n");
    }

}

void authors(){
    printf("Ángela García Sanchez (angela.gsanchez@udc.es)\nXabier Guitián López (x.guitian@udc.es)\n");
}

void processCommand(){

}

int main(){
    char comando[MAX_COMMAND_LENGHT];

    leerComando(comando);
    addHistoric(comando);


    return 0;
}