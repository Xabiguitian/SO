//
// Created by angela on 22/10/24.
//

//LIBRERÍAS IMPLEMENTADAS
#include <stdio.h>
#include "p0.h"
#include "p1.h"
#include "historial.h"


//FUNCIONES DE LOS COMANDOS

//Imprime el nombre y los logins de los autores del núcleo
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

/*Cambia el directorio de trabajo actual del shell a dir .Cuando se invoca sin aumentos,
imprime el directorio de trabajo actual .*/
void cd(char * trozos[]){
	char cwd[MAXIMUN];

	if(trozos[1]==NULL){
		if(getcwd(cwd,sizeof cwd)!=NULL){
    		printf("%s\n",cwd);
    	}else{
        	printf("No se puede mostrar el directorio\n");
    	}
		return;
	}else if(strcmp(trozos[1],"..")==0){
		if(chdir("..")!=0){
			perror("Error: falta la direccion\n");
		}
	}else{
		if(chdir(trozos[1])!=0)
			perror("No se puede mostrar el directorio\n");
	}
}

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

 //ayuda muestra una lista de comandos disponibles. ayuda cmd da una breve ayuda
    //sobre el uso del comando cmd
void help(tList *historial, char * trozos[]){
  if(trozos[1]==NULL){
    printf("Comandos disponibles: authors\npid\nppid\ncd\ndate\nopen\ndup\nclose\ninfosys\nquit\nhistoric\nmakefile\nmakedir\ncwd\nlistfile\nlistdir\nreclist\nrevlist\nerase\ndelrec\nquit\nexit\nbye\n");
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
  }else if(strcmp(trozos[1], "makefile")==0) {
    printf("Crea un archivo\n");
  }else if(strcmp(trozos[1],"makedir")==0) {
    printf("Crea un directorio\n");
  }else if(strcmp(trozos[1],"cwd")==0) {
    printf("Imprime la ruta del directorio actual\n");
  }else if(strcmp(trozos[1],"listfile")==0) {
    printf("Proporciona información de archivos y directorios\n");
  }else if(strcmp(trozos[1],"listdir")==0) {
    printf("Lista el contenido de un directorio");
  }else if(strcmp(trozos[1],"reclist")==0) {
    printf("Lista los directorios recursivamente (subdirectorios después)\n");
  }else if(strcmp(trozos[1],"revlist")==0) {
    printf("Lista los directorios recursivamente (subdirectorios antes)\n");
  }else if(strcmp(trozos[1],"erase")==0) {
    printf("Elimina archivos y directorios vacíos\n");
  }else if(strcmp(trozos[1],"delrec")==0) {
    printf("Elimina archivos y directorios tanto que estén vacíos como no (eliminando los respectivos subdirectorios)\n");
  }else
    printf("No se reconoce el comando.\n");
}


//Imprime información en la máquina que ejecuta el shell (obtenida a través de
// la llamada al sistema/función de biblioteca uname)
void infosys(){
    struct utsname infosys;

    if(uname(&infosys)==0)
        printf("Información de la máquina en la que se ejecuta el shell\nArqitectura del sistema:%s\n Sistema operativo:%s,Versión del núcleo:%s\nNombre del host:%s\n",infosys.machine,infosys.sysname,infosys.release, infosys.nodename);
    //PREGUNTAR COMO DIVIDIR EL PRINTF EN DOS LINEAS!!
}


//función para salir del shell.
void off(){
    exit(0);
}


//FUNCIÓN QUE MUESTRA EL HISTORIAL DEL SHELL
/*void cmdhistoric(char *trozos[],tList * historial){
    int i;
    int N = (trozos[1] != NULL) ? strtol(trozos[1], NULL, 10) : 0;

    if(trozos[1]==NULL){
        for(i=0;i<=last(*historial);i++){
            printf("%s\n",  getItemH(i,*historial));
        }
    }else if(strcmp(trozos[1],"-")==0){
        for(i=N;i<=last(*historial);i++){
            printf("%s\n",getItemH(i,*historial));
        }
    }else { // N solo
        printf("%s\n",getItemH(N,*historial));
        }else{
            printf("No se reconoce el comando.\n");
    }*/


