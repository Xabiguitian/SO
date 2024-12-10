

//LIBRERÍAS IMPLEMENTADAS
#include <stdio.h>
#include "p0.h"
#include "p1.h"
#include "historial.h"
#include "file.h"


//FUNCIONES DE LOS COMANDOS
//Como está implementada en el main la ponemos otra vez aqui apra que el programa la reconozca
int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

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
    printf("Lista el contenido de un directorio\n");
  }else if(strcmp(trozos[1],"reclist")==0) {
    printf("Lista los directorios recursivamente (subdirectorios después)\n");
  }else if(strcmp(trozos[1],"revlist")==0) {
    printf("Lista los directorios recursivamente (subdirectorios antes)\n");
  }else if(strcmp(trozos[1],"erase")==0) {
    printf("Elimina archivos y directorios vacíos\n");
  }else if(strcmp(trozos[1],"delrec")==0) {
    printf("Elimina archivos y directorios tanto que estén vacíos como no (eliminando los respectivos subdirectorios)\n");
  }else if(strcmp(trozos[1],"recurse")==0){
    printf("ejecuta la función recursiva n veces.\n");
  }else if(strcmp(trozos[1], "memory")==0){
      printf("mem [-blocks|-funcs|-vars|-all|-pmap] ...\nMuestra detalles de la memoria del proceso\nblocks: los bloques de memoria asignados\n-funcs: las direcciones de las funciones\n-vars: las direcciones de las variables\n-all: todo\npmap: muestra la salida del comando pmap(o similar)\n");
  }else if(strcmp(trozos[1], "read")==0){
    printf("Lo mismo que readfile pero usamos un descriptor de archivo (ya abierto)\n");
  }else if(strcmp(trozos[1], "write")==0){
    printf("Lo mismo que writefile pero usamos un descriptor de archivo (ya abierto)\n");
  }else if(strcmp(trozos[1], "readfile")==0){
    printf("Lee cont bytes de un archivo en la dirección de memoria addr\n");
  }else if(strcmp(trozos[1], "writefile")==0){
    printf("escribe en un archivo cont bytes comenzando en la dirección de memoria addr\n");
  }else if(strcmp(trozos[1], "memdump")==0){
      printf("Vuelca el contenido de cont bytes de memoria en la dirección addr a la pantalla. Vuelca códigos hexadecimales y, para caracteres imprimibles, el carácter asociado\n");
  }else if(strcmp(trozos[1], "memfill")==0){
      printf("dumps llena la memoria con el carácter ch, comenzando en la dirección addr, para con tbytes\n");
  }else
    printf("No se reconoce el comando.\n");
}


//Imprime información en la máquina que ejecuta el shell (obtenida a través de
// la llamada al sistema/función de biblioteca uname)
void infosys(){
    struct utsname infosys;

    if(uname(&infosys)==0)
        printf("Información de la máquina en la que se ejecuta el shell\nArqitectura del sistema:%s\n Sistema operativo:%s\nVersión del núcleo:%s\nNombre del host:%s\n",infosys.machine,infosys.sysname,infosys.release, infosys.nodename);
    //PREGUNTAR COMO DIVIDIR EL PRINTF EN DOS LINEAS!!
}


//función para salir del shell.
void off(){
    exit(0);
}


//Cambia el directorio de trabajo actual del shell a dir .Cuando se invoca sin aumentos, imprime el
// directorio de trabajo actual .
void cd(char * trozos[]){
  if(trozos[1]==NULL){
    cwd();
  }else{
    if(chdir(trozos[1])!=0){
      perror("No se puede cambiar el directorio.\n");
    }
  }
}

//FUNCIÓN QUE MUESTRA EL HISTORIAL DEL SHELL
void cmdhistoric(char *trozos[],tList * historial){
    int i,j;

    if(trozos[1]==NULL){
        for(i=0;i<=last(*historial);i++){
            printf("%s\n",  getItemH(i,*historial));
        }
    }else if(atoi(trozos[1])==0){
      perror("No hay comando número 0 en el histórico.\n");

    }else if(atoi(trozos[1])>0){
      if(atoi(trozos[1])>last(*historial)){
        perror("No se han introducido tantos comandos en el sistema.\n");
      }else{

        printf("%s\n",  getItemH(atoi(trozos[1])-1,*historial));
      }

    }else if((atoi(trozos[1])<0)){
      if(abs(atoi(trozos[1]))>last(*historial)+1){
        perror("No se han introducido tantos comandos en el sistema.\n");
      }else{
        j=abs(atoi(trozos[1]));
        for(i=last(*historial)-j+1;i<=last(*historial);i++){
          printf("%s\n",  getItemH(i,*historial));
        }
      }
    }
}









void Cmd_open(char * tr[], filelist * F){
  int i,df, mode=0;

  if (tr[1]==NULL) {
    listarFicheros(F);
    return;
  }
  for (i=2; tr[i]!=NULL; i++)
    if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
    else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
    else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY;
    else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
    else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
    else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
    else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC;
    else break;

  if ((df=open(tr[1],mode,0777))==-1)
    perror ("Imposible abrir fichero");
  else{
    printf ("Anadida entrada a la tabla ficheros abiertos..................\n");
    añadirFicheros(df, tr[1], mode, F);
}
}


//FUNCION CLOSE
void Cmd_close(char *tr[],filelist *F){
    int df;

    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
      listarFicheros(F);

    } else {
      df=atoi(tr[1]);
      if (close(df)==-1)
        perror("Imposible cerrar descriptor");
      else{
	      EliminarFichero(F, df);
      }
    }
}

//FUNCION DUP
void Cmd_dup (char * tr[],filelist *F){
    int df, duplicado;
    char aux[MAXNAME],*p;

    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
        listarFicheros(F);
        return;
    }

    df=atoi(tr[1]);
    duplicado=dup(df);
    if(duplicado==-1){
      perror("Error al duplicar el descriptor\n");
    }

    p=getItemF(df,*F);
    if(p==NULL){
      perror("No se encontró el archivo asociado al descriptor.\n");
    }

    tFile newFile;
    newFile.id=duplicado;
    newFile.mode=fcntl(duplicado,F_GETFL);

    if(p ==NULL || duplicado == -1){
      perror("Error al añadir el archivo duplicado a la lista de archivos abiertos.\n");
    } else {
      sprintf(aux, "dup %d (%s)", df, p);
      añadirFicheros(duplicado, aux, newFile.mode, F);
    }
}
