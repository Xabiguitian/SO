#include <stdio.h>

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


}


void processCommand(){
}
int main(){
    char comando[MAX_COMMAND_LENGHT];

    leerComando(comando);
    addHistoric(comando);


    return 0;
}

