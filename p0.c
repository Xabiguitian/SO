//
// Angela Garcia Sánchez y Xabier Guitián López
//

//LIBRERÍAS IMPLEMENTADAS
#include "historial.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


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
void cmdate(char *trozos[]);
void cmdopen();
//void cmddup();
void infosys();
void off();
void cmdhistoric(char *trozos[], tList * historial);
void cmdclose(char *trozos[]);
void help(tList *historial, char * trozos[]);

//P1
void makefile(char *trozos[]);
void makedir(char *trozos[]);
void printPermissions(struct stat fileStat);
void listFile(char *trozos[]);
void listDir(char *trozos[]);
void cwd();



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
        cmdopen();
    }else if(strcmp(command, "dup")==0) {
        //cmddup();
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

  if(trozos[1]==NULL){
      perror("Error: falta el parámetro\n");
      return;
  }else{
    if(chdir(trozos[1])!=0)
    	perror("No se puede mostrar el directorio\n");
	}
}
//HECHA MAS O MENOS LA FUNCIÓN DE CD!!!!






//Imprime la fecha actual en el formato DD/MM/AAAA y la fecha actual (-d) hora en el formato hh:mm:ss (-t)
void cmdate(char *trozos[]){
    time_t t=time(NULL);
    struct tm *tm_info = localtime(&t);
    int dia = tm_info->tm_mday;
    int mes = tm_info->tm_mon+1;
    int ano = tm_info->tm_year+1900;  int horas = tm_info->tm_hour;
    int minutos = tm_info->tm_min;
    int segundos = tm_info->tm_sec;

    if (trozos[1]==NULL){
        printf("Fecha: ");
        if(dia<10 && mes <10){
            printf("0%d/0%d/%d\n",dia , mes ,ano);
        }else if (dia <10){
            printf("0%d/%d/%d\n",dia , mes ,ano);
        }else if (mes <10){
            printf("%d/0%d/%d\n",dia , mes ,ano);
        }else {
            printf("%d/%d/%d\n", dia, mes, ano);
        }

        printf("Hora: ");
        if(horas<10 && minutos <10 && segundos<10){
            printf("0%d:0%d:0%d\n", horas, minutos, segundos);
        }else if(horas<10 && minutos <10){
            printf("0%d:0%d:%d\n", horas, minutos, segundos);
        }else if(horas<10 && segundos<10){
            printf("0%d:%d:0%d\n", horas, minutos, segundos);
        }else if(minutos <10 && segundos<10){
            printf("%d:0%d:0%d\n", horas, minutos, segundos);
        }else if (horas <10){
            printf("0%d:%d:%d\n",horas , minutos ,segundos);
        }else if (minutos <10){
            printf("%d:0%d:%d\n",horas , minutos ,segundos);
        }else if(segundos<10){
            printf("%d:%d:0%d\n", horas, minutos, segundos);
        }else {
            printf("%d:%d:%d\n", horas, minutos, segundos);
        }

    }else if(strcmp(trozos[1], "-d")==0 ){
        printf("Fecha: ");
        if(dia<10 && mes <10){
            printf("0%d/0%d/%d\n",dia , mes ,ano);
        }else if (dia <10){
            printf("0%d/%d/%d\n",dia , mes ,ano);
        }else if (mes <10){
            printf("%d/0%d/%d\n",dia , mes ,ano);
        }else {
            printf("%d/%d/%d\n", dia, mes, ano);
        }
    }else if(strcmp(trozos[1], "-t")==0 ) {
        printf("Hora: ");
        if(horas<10 && minutos <10 && segundos<10){
            printf("0%d:0%d:0%d\n", horas, minutos, segundos);
        }else if(horas<10 && minutos <10){
            printf("0%d:0%d:%d\n", horas, minutos, segundos);
        }else if(horas<10 && segundos<10){
            printf("0%d:%d:0%d\n", horas, minutos, segundos);
        }else if(minutos <10 && segundos<10){
            printf("%d:0%d:0%d\n", horas, minutos, segundos);
        }else if (horas <10){
            printf("0%d:%d:%d\n",horas , minutos ,segundos);
        }else if (minutos <10){
            printf("%d:0%d:%d\n",horas , minutos ,segundos);
        }else if(segundos<10){
            printf("%d:%d:0%d\n", horas, minutos, segundos);
        }else {
            printf("%d:%d:%d\n", horas, minutos, segundos);
        }

    }else{
        printf("No se reconoce el comando. \n");
    }
}



//FUNCIÓN QUE MUESTRA EL HISTORIAL DEL SHELL
void cmdhistoric(char *trozos[], tList *historial) {
    if (trozos[1] == NULL) {
        int i;
        for (i = 0; i <= last(*historial); i++) {
            printf("%d->%s", i, getItemH(i, *historial));
        }
    }
    else if (trozos[1][0] == '-' && (trozos[1][1] >= '0' && trozos[1][1] <= '9')) {
        long int p = strtol(&trozos[1][1], NULL, 10);
        if (p > 0 && p <= last(*historial)) {
            int i;
            for (i = last(*historial) - p + 1; i <= last(*historial); i++) {
                printf("%d->%s", i, getItemH(i,*historial));
            }
        } else {
            printf("No hay suficientes elementos en el historial para mostrar los últimos %ld comandos.\n", p);
        }
    }
    else if (trozos[1][0] >= '0' && trozos[1][0] <= '9') {
        long int p = strtol(trozos[1], NULL, 10);
        if (p <= last(*historial)) {
            printf("El comando %ld es: %s", p, getItemH(p, *historial));
        } else {
            printf("No existe %ld en el histórico.\n", p);
        }
    }
    // Comando no válido
    else {
        printf("Comando no reconocido.\n");
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

void cmdopen(){

}




    //Duplica el descriptor del archivo df (usando la llamada al sistema dup, creando el
    //nueva entrada correspondiente en la lista de archivos
    /*void cmddup(char * trozos[]){
    int df, duplicado;
    char aux[MAXNAME],*p;

    if (trozos[0]==NULL || (df=atoi(trozos[0]))<0) { no hay parametro
        ListOpenFiles(-1);                 o el descriptor es menor que 0
        return;
    }

    duplicado=dup(df);
   p=.....NombreFicheroDescriptor(df).......;
    sprintf (aux,"dup %d (%s)",df, p);
    //.......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
}
    }*/


void makefile(char *trozos[]){

    if (trozos[1] == NULL) {
       printf("Error al crear el archivo %s\n", trozos[1]);
    }

    FILE *file = fopen(trozos[1], "w");

    if (file == NULL) {
      perror("Error al abrir el archivo");
      return;
    }

    printf("Archivo %s creado exitosamente\n", trozos[1]);
    fclose(file);

}

void makedir(char *trozos[]){
    if (trozos[1] == NULL) {
        printf("Error al crear el directorio %s\n", trozos[1]);
    }

    if (mkdir(trozos[1], 0700) == -1) {
        perror("Error al crear el directorio");
        return;
    }

    printf("Directorio %s creado exitosamente\n", trozos[1]);
}

void printPermissions(struct stat fileStat){
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
}

void listFile(char *trozos[]){
    struct stat fileStat;
    char *nombre, cwd[MAXIMUN];

    if (trozos[1] == NULL){
        getcwd(cwd,sizeof cwd);
        nombre = cwd;
    } else {
        nombre = trozos[1];
    }

    if (stat(nombre, &fileStat) == -1){
        perror("Error al obtener información del archivo o directorio");
        return;
    }

    printf("Información sobre: %s\n", nombre);
    printf("Tamaño: %lld bytes\n", fileStat.st_size);
    printf("Permisos: ");
    printPermissions(fileStat);
    printf("\nNúmero de enlaces: %hd\n", fileStat.st_nlink);
    printf("Propietario: %s\n", getpwuid(fileStat.st_uid)->pw_name);
    printf("Grupo: %s\n", getgrgid(fileStat.st_gid)->gr_name);
    printf("Último acceso: %s\n", ctime(&fileStat.st_atime));
    printf("Última modificación: %s", ctime(&fileStat.st_mtime));
}

void listDir(char *trozos[]){
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *dirpath;

    if (trozos[1] == NULL){
        dirpath = ".";
    } else {
        dirpath = trozos[1];
    }

    dir = opendir(dirpath);
    if (dir == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("Archivos y directorios en %s:\n", dirpath);
    while ((entry = readdir(dir)) != NULL) {
        char filepath[MAX];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);

        if(stat(filepath, &fileStat) == -1){
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)){
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("%s\n", entry->d_name);
            }
        }

    }

    closedir(dir);
}

void cwd(){
    char cwd[MAXIMUN];
    getcwd(cwd,sizeof cwd);
    printf("%s\n",cwd);
}