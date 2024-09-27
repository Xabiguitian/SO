//LIBRERÍAS IMPLEMENTADAS
#include "historial.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/utsname.h>


//CONSTANTES DEFINIDAS
#define MAX 1024
#define MAXIMUNT 3


//INDICE DE FUNCIONES DE LOS COMANDOS
void imprimirPrompt();
void leerComando(char *command);
void processCommandTrocearCadena(char command[],tList *historial, char * trozos[]);

// COMANDOS P0
void authors(char *trozos[]);
void pid();
void ppid();
void cd(char * trozos[]);
void cmddate(char * trozos[]);
void cmdopen();
//void cmddup();
void infosys();
void off();
void cmdhistoric(char *trozos[], tList * historial);
void cmdclose(char *trozos[]);
void help(tList *historial, char * trozos[]);





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
int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
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
        cmddate(trozos);
    }else if(strcmp(command,"open")==0) {
        cmdopen();
    }else if(strcmp(command, "dup")==0) {
        cmddup();
    }else if(strcmp(command, "close")==0) {
        cmdclose(trozos);
    }else if(strcmp(command,"infosys")==0) {
        infosys();
    }else if(strcmp(command,"help")==0) {
        help(historial, trozos);
    }else if(strcmp(command,"quit")==0||strcmp(command,"exit")==0||strcmp(command,"bye")==0) {
        off();
    }else if(strcmp(command,"historic")==0) {
        cmdhistoric(trozos,historial);
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








//FUNCIONES DE LOS COMANDOS
void authors(char *trozos[]){
    if(trozos[1]==NULL) {
        printf("Ángela García Sánchez (angela.gsanchez@udc.es)\nXabier Guitián López (x.guitian@udc.es)\n");
    }else if(strcmp(trozos[1],"-l")==0) {
        printf("angela.gsanchez@udc.es\nx.guitian@udc.es\n");
    }else if(strcmp(trozos[1],"-n")==0){
        printf("Ángela García Sánchez\nXabier Guitián López\n");

    }else {
        printf("No se reconoce el comando.\n");
    }

}



//Imprime el pid del proceso que se está ejecutando en el nucleo
void pid(){
    printf("PID: %d\n", getpid());
}


//Imprime el pid del padre del proceso del núclep.
void ppid(){
    printf("PPID: %d\n", getppid());
}



//Cambia el directorio de trabajo actual del shell a dir .Cuando se invoca sin aumentos, imprime el
// directorio de trabajo actual .
void cd(char * trozos[]){
    char cwd[MAXIMUN];

    if(trozos[1]==NULL){
        if(getcwd(cwd,sizeof cwd)!=NULL){
            printf("%s\n",cwd);
        }else{
            printf("No se puede mostrar el directorio\n");
        }
    }else{
        if(chdir(trozos[1])!=0)
            perror("No se puede mostrar el directorio\n");
    }
}
//HECHA MAS O MENOS LA FUNCIÓN DE CD!!!!






//Imprime la fecha actual en el formato DD/MM/AAAA y la fecha actual (-d) hora en el formato hh:mm:ss (-t)
void cmddate(char *trozos[]){
    time_t t=time(NULL);
    struct tm *tm_info = localtime(&t);

    if(strcmp(trozos[1], "-d")==0 || trozos[1] == NULL){
        int dia = tm_info->tm_mday;
        int mes = tm_info->tm_mon+1;
        int ano = tm_info->tm_year+1900;
        printf("%d/%d/%d\n",dia , mes ,ano);
    }else if(strcmp(trozos[1], "-t")==0 || trozos[1] == NULL) {
        int horas = tm_info->tm_hour;
        int minutos = tm_info->tm_min;
        int segundos = tm_info->tm_sec;

        printf("%d:%d:%d\n", horas, minutos, segundos);

    }else if (trozos[1]==NULL){
        printf("No se reconoce el comando. \n");
    }else{
        printf("No se reconoce el comando. \n");
    }

}



//FUNCIÓN QUE MUESTRA EL HISTORIAL DEL SHELL
void cmdhistoric(char *trozos[],tList * historial){
    int i;
    int N = strtol(trozos[1], NULL, 10);
    if(trozos[1]==NULL){
        for(i=0;i<last(*historial);i++){
            printf("%s\n",  getItemH(i,*historial));
        }
    }else if(strcmp(trozos[1],"-")==0){
        for(i=N;i<last(*historial);i++){
            printf("%s\n",getItemH(i,*historial));
        }
    }else { // N solo
        printf("%s\n",getItemH(N,*historial));
        /*}else{
            printf("No se reconoce el comando.\n");*/
    }
}


//ayuda muestra una lista de comandos disponibles. ayuda cmd da una breve ayuda
//sobre el uso del comando cmd
void help(tList *historial, char * trozos[]){
    if(trozos[1]==NULL){
        printf("Comandos disponibles: ...\n");
    }
    else if(strcmp(trozos[1], "authors")==0) {
        printf("Enseña el nombre, apellidos y de los logins de los autores.\n");
    }else if(strcmp(trozos[1],"pid")==0) {
        printf("Enseña el pid del proceso que se está ejecutando en el shell.\n");
    }else if(strcmp(trozos[1],"ppid")==0) {
        printf("Enseña el pid del proceso padre del shell\n");
    }else if(strcmp(trozos[1],"cd")==0) {
        printf("Cambia el directorio de trabajo actual del shell a dir (usando el llamada al sistema chdir). Cuando se invoca sin aumentos, imprime el directorio de trabajo actual (usando la llamada al sistema getcwd.\n");
    }else if(strcmp(trozos[1],"date")==0){
        printf("Enseña la hora o la fecha actual o ambas\n");
    }else if(strcmp(trozos[1],"open")==0) {
        printf("Abre un archivo y lo agrega (junto con el descriptor del archivo y el modo de apertura a la lista de archivos abiertos del shell. Para el modo usaremos cr para O CREAT, ap para O APPEND, ex para O EXCL, ro para O RDONLY, rw para O RDWR, wo para O WRONLY y tr para O TRUNC.\nAbrir sin argumentos enumera los archivos abiertos del shell. Para cada archivo enumera su descriptor, el nombre del archivo y el modo de apertura. \n");
    }else if(strcmp(trozos[1], "dup")==0) {
        printf("Duplica el descriptor del archivo df (usando la llamada al sistema dup, creando el nueva entrada correspondiente en la lista de archivos\n");
    }else if(strcmp(trozos[1], "close")==0) {
        printf("Cierra el descriptor del archivo df y elimina el elemento correspondiente de la lista\n");
    }else if(strcmp(trozos[1],"infosys")==0) {
        printf("Imprime la información de la máquina donde se ejecuta el shell.\n");
    }else if(strcmp(trozos[1],"quit")==0||strcmp(trozos[1],"exit")==0||strcmp(trozos[1],"bye")==0) {
        printf("Sale del shell.\n");
    }else if(strcmp(trozos[1],"historic")==0) {
        printf("Enseña el historial de los comando usados en el shell.\nHistoric N, imprime los N últimos elementos del historic.\nHistoric N imprime el -N elemento del historial.\n");
    }else
        printf("No se reconoce el comando.\n");
}




//Imprime información en la máquina que ejecuta el shell (obtenida a través de
// la llamada al sistema/función de biblioteca uname)
void infosys(){
    struct utsname infosys;

    if(uname(&infosys)==0)
        printf("Información de la máquina en la que se ejecuta el shell\nArqitectura del sistema:%s\n Sistema operativo:%s,Versión del núcleo:%s,Nombre del host:%s\n",infosys.machine,infosys.sysname,infosys.release, infosys.nodename);
    //PREGUNTAR COMO DIVIDIR EL PRINTF EN DOS LINEAS!!
}







//función para salir del shell.
void off(){
    exit(0);
}








//Cierra el descriptor del archivo df y elimina el elemento correspondiente de la lista
void cmdclose(char *trozos[]){
    int df;

    if (trozos[0]==NULL || (df=atoi(trozos[0]))<0) { /*no hay parametro*/
        //..............ListarFicherosAbiertos............... /*o el descriptor es menor que 0*/
        return;
    }


    if (close(df)==-1)
        perror("Imposible cerrar descriptor");
    else{
        // ........EliminarDeFicherosAbiertos......
    }
}

void cmdopen (char * tr[])
{
    int i,df, mode=0;

    if (tr[0]==NULL) { /*no hay parametro*/
        ..............ListarFicherosAbiertos...............
        return;
    }
    for (i=1; tr[i]!=NULL; i++)
        if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
        else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
        else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY;
        else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
        else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
        else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
        else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC;
        else break;

    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        ...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        printf ("Anadida entrada a la tabla ficheros abiertos..................",......);
    }





//Duplica el descriptor del archivo df (usando la llamada al sistema dup, creando el
//nueva entrada correspondiente en la lista de archivos
void cmddup(char * trozos[]){
    int df, duplicado;
    char aux[MAXNAME],*p;

    if (trozos[0]==NULL || (df=atoi(trozos[0]))<0) { no hay parametro
        ListOpenFiles(-1);                 //o el descriptor es menor que 0
        return;
    }

    duplicado=dup(df);
    p=.....NombreFicheroDescriptor(df).......;
    sprintf (aux,"dup %d (%s)",df, p);
    //.......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
}