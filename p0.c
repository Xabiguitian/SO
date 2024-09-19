#include <stdio.h>

void printPrompt(){
    printf("# ");
}

void readInput(char *input){

    fgets(input,sizeof(input), stdin);
}


void createHistoric(char* historic){
    for (int i = 0; i < 4096; ++i) {
        historic[i] = -1;
    }

}

void printHistoric(char *historic){
    int i=0;

    while (historic[i] != -1){
        printf("%s", &historic[i]);
        i++;
    }

}

void addHistoric(char *historic, char command){
    int i=0;

    while (historic[i] != -1){
        i++;
    }

    historic[i] = command;
}




int main (){
    char input[50];
    char historic[4096];
    createHistoric(historic);

    printf("%s", &historic[0]);

    return 0;
}