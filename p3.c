//
// Created by angela on 3/12/24.
//
#include "p3.h"



//función auxiliar para obtener el UID
uid_t userUID(char *login) {
  struct passwd *p;

  if((p = getpwnam(login)) == NULL)
    return (uid_t) -1;

  return p->pw_uid;
}


char * username(uid_t uid) {
    struct passwd *p;

    if((p= getpwuid(uid)) == NULL)
        return ("?????");
    return p->pw_name;
}

void getUid(){
  uid_t real = getuid(), efectiva = geteuid();

  printf("Credencial real: %d, (%s)\n", real, username(real));
  printf("Credencial efectival: %d, (%s)\n", efectiva, username(efectiva));
}


void cmd_setUid(char *trozos[]) {
  uid_t uid;


  if (strcmp(trozos[1], "-l") == 0) {

    if ((uid = userUID(trozos[2])) == (uid_t)-1) {
      printf("Usuario no existente: %s\n", trozos[2]);
      return;
    }
  } else {

    uid = (uid_t) atoi(trozos[1]);
  }

  if (setuid(uid) == -1) {
    perror("Imposible cambiar credencial");
  } else {
    printf("Credencial efectiva cambiada a UID: %d\n", uid);
  }
}





void Cmd_fork (char * tr[], tListProc * ListProc) {
    pid_t pid;

    if ((pid=fork())==0){
        deleteProcList(ListProc);
        printf ("ejecutando proceso %d\n", getpid());
    }
    else if (pid!=-1)
        waitpid (pid,NULL,0);
}
void Cenviron(char *trozos[], char * arg3[], char *env[]){

  if (trozos[1] == NULL) {
    printVar(arg3, "main arg3");
  }else if (! strcmp(trozos[1],"-addr")) {
    printf("environ: %p (almacenado en %p)\n", env, &env);
    printf("main arg3: %p (almacenado en %p)\n", arg3, &arg3);
  }else if (! strcmp(trozos[1],"-environ")) {
    printVar(env, "environ");


  }else
    printf("Parámetros incorrectos\n");
}






// Codigo dado por los profesores:
int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parÃ¡metro*/
{
  int pos=0;
  char aux[MAXVAR];

  strcpy (aux,var);
  strcat (aux,"=");

  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else
      pos++;
  errno=ENOENT;   /*no hay tal variable*/
  return(-1);
}
int CambiarVariable(char * var, char * newVar, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parÃ¡metro*/
{                                                        /*lo hace directamente, no usa putenv*/
  int pos;
  char *aux;

  if ((pos=BuscarVariable(var,e))==-1)
    return(-1);

  if ((aux=(char *)malloc(strlen(newVar)+strlen(valor)+2))==NULL)
    return -1;
  strcpy(aux,newVar);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}



void printVar(char *env[], char * name) {
  int i;

  for(i = 0; env[i] != NULL; i++)
    printf("%p->%s[%d]=(%p) %s\n", &env[i], name, i, env[i], env[i]);
}



void subsvar(char *trozos[], char *arg3[], char *env[]) {
  int cont;
  char *var;
  for (cont = 0; trozos[cont] != NULL; cont++);

  if (cont == 5) {
    var= malloc(strlen(trozos[3])+ strlen(trozos[4])+2);
    strcpy(var, trozos[3]);
    strcat(var, "=");
    strcat(var, trozos[4]);

    if (!strcmp(trozos[1],"-a")) {
      if (CambiarVariable( trozos[2],trozos[3],trozos[4],arg3)==-1) {
        free(var);
        perror("Imposible cambiar variable");
        return;
      }
    }else if (!strcmp(trozos[1],"-e")) {
      if (CambiarVariable(trozos[2],trozos[3], trozos[4],env)==-1) {
        free(var);
        perror("Imposible cambiar variable");
        return;
      }
    }else {
      free(var);
      printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
    }
  }else
    printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
}

void changevar(char *trozos[], char * arg3[], char * env[]) {
int cont;
  char *var;

  for(cont = 0; trozos[cont] != NULL; cont++);
  if (cont==4) {
    var=malloc(strlen(trozos[2])+strlen(trozos[3])+4);
    strcpy(var, trozos[2]);
    strcat(var, "=");
    strcat(var, trozos[3]);

    if (!strcmp(trozos[1],"-a")) {
      if (CambiarVariable(trozos[2],trozos[2],trozos[3],arg3)==-1) {
        free(var);
        perror("Imposible cambiar variable");
        return;
      }
    }else if (!strcmp(trozos[1],"-e")) {
      if (CambiarVariable(trozos[2],trozos[2],trozos[3],env)==-1) {
        free(var);
        perror("Imposible cambiar variable");
        return;
      }
    }else if (!strcmp(trozos[1],"-p")) {
      if(putenv(var) != 0) {
        free(var);
        perror("Imposible cambiar variable");
        return;
      }
    }else {
      free(var);
      printf("Uso: changevar [-a|-e|-p] var valor\n");
      return;
    }
  }else
    printf("Uso: changevar [-a|-e|-p] var valor\n");
}

void showvar(char * trozos[], char *arg3[], char * env[]) {
int i , j;
char *valor;
  if (trozos[1]==NULL)
    printVar(arg3, "main arg3");
  else {
    valor=getenv(trozos[1]);
    if (valor==NULL)
      return;
    if ((i=BuscarVariable(trozos[1],arg3))==-1) {
      perror("Error al obtener main arg3\n");
      return;
    }
    if ((j=BuscarVariable(trozos[1],environ))==-1) {
      perror("Error al obtener el environ\n");
      return;
    }

    printf("Con arg3 main %s(%p) @%p\n", arg3[i], arg3[i], &arg3[i]);
    printf("Con environ %s(%p) @%p\n", environ[j], environ[j], &environ[j]);
    printf("Con getenv %s(%p)\n", valor, &valor);
  }
}

void execCmd(char *tr[], char *input, tList *hist, tListM *M, tListProc *ListProc, char *envp[]) {
  if (tr[1] == NULL) {
    printf("Uso: exec VAR1 VAR2 ..prog args....[@pri]\n");
  } else {
    int cnt = 0;
    char *tr2[MAXTROZOS];
    int n = TrocearCadena(input, tr2);

    while (tr[cnt + 1] != NULL)
      cnt++;
    if (execvp(tr[1], tr + 1) == -1) {
      perror("Imposible ejecutar el programa");
    }
  }
}




void execpri(char *tr[], char *input, tList *hist, tListM *M, tListProc *ListProc, char *envp[]) {
  if (tr[1] == NULL) {
    printf("Uso: execpri prio progspec\n");
  } else {
    int cnt = 0;
    char *tr2[MAXTROZOS];
    int n = TrocearCadena(input, tr2);

    while (tr[cnt + 1] != NULL)
      cnt++;

    if (strstr(tr[n - 1], "@") != NULL) {
      if (setpriority(PRIO_PROCESS, getpid(), atoi(tr[cnt] + 1)) == -1) {
        perror("Error al cambiar la prioridad");
        return;
      }
      tr2[n - 1] = NULL;
      execCmd(tr, input, hist, M, ListProc, envp);
    } else {
      execCmd(tr, input, hist, M, ListProc, envp);
    }
  }
}

/*void fg(char *trozos[], char *progspec) {

}*/
