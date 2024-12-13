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

int Execpve(char *tr[], char **NewEnv, int *pprio, tSearchList LibroDeBusqueda) {
    char *p = Ejecutable(tr[0], LibroDeBusqueda);
    if (p == NULL) {
        fprintf(stderr, "Error: No se encontró el ejecutable '%s'.\n", tr[0]);
        errno = ENOENT;
        return -1;
    }

    if (pprio != NULL && setpriority(PRIO_PROCESS, getpid(), *pprio) == -1) {
        perror("Imposible cambiar prioridad");
        return -1;
    }

    if (NewEnv == NULL) {
        return execv(p, tr);
    } else {
        return execve(p, tr, NewEnv);
    }
}

void execCmd(char *trozos[], tSearchList LibroDeBusqueda) {
    char *args[MAXTROZOS];  // Arreglo para almacenar los argumentos
    int i = 0;

    // Asegúrate de que el primer argumento no sea NULL
    if (trozos[0] == NULL) {
        printf("Error: Comando no válido.\n");
        return;
    }

    // Copiar los trozos del comando en el arreglo de argumentos
    while (trozos[i] != NULL) {
        args[i] = trozos[i];
        i++;
    }
    args[i] = NULL;  // El último argumento debe ser NULL, como requiere execve

    // Ahora, llamamos a Execpve para ejecutar el comando
    if (Execpve(args, NULL, NULL, LibroDeBusqueda) == -1) {
        // Si Execpve devuelve -1, hubo un error al intentar ejecutar el comando
        perror("Error al ejecutar el comando");
    }
}


void execpri(char *tr[], char *input, tList *hist, tListM *M, tListProc *ListProc, char *envp[], tSearchList LibroDeBusqueda) {
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
      execCmd(tr, LibroDeBusqueda);
    } else {
      execCmd(tr, LibroDeBusqueda);
    }
  }
}

void back(char *trozos[], tListProc *listProc, tSearchList *LibroDeBusqueda) {
    pid_t pid;
    char *path;

    if (trozos[1] == NULL) {
        printf("Uso: back comando args\n");
        return;
    }

    path = Ejecutable(trozos[1],*LibroDeBusqueda);

    dataProc newProc;
    newProc.user = strdup(username(getuid()));
    newProc.cmd = strdup(trozos[1]);

    time_t currentTime = time(NULL);
    newProc.date = strdup(ctime(&currentTime));

    if (path == NULL || access(path, X_OK) == -1) {
        newProc.pid = 0; // PID ficticio
        newProc.estado = FINISHED;
        newProc.end = 255; // Código de salida por defecto
        printf("No ejecutado: No such file or directory\n");
        insertItemProcList(newProc, listProc);
        return;
    }

    if ((pid = fork()) == 0) {
        // Proceso hijo: ejecutar el comando
        char *args[] = {path, NULL};
        execve(path, args, environ);
        perror("No ejecutado");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Proceso padre: esperar y registrar
        newProc.pid = pid;
        newProc.estado = ACTIVE;
        newProc.end = 0; // Código de salida no definido aún

        if (!insertItemProcList(newProc, listProc)) {
            printf("Error: No se pudo añadir el proceso a la lista\n");
        } else {
            // Esperar al proceso y registrar salida
            waitpid(pid, &newProc.end, 0);
            newProc.estado = FINISHED;

            // Mostrar directamente la salida del comando
            printf("\n");
            fflush(stdout);
        }
        updateItemProcList(newProc, lastProcList(*listProc), listProc);
    } else {
        perror("Error al crear proceso en background");
    }
}


void backpri(char *trozos[], tListProc *listProc, tSearchList LibroDeBusqueda) {
    pid_t pid;
    char *path;

    if (trozos[1] == NULL || trozos[2] == NULL) {
        printf("Uso: backpri prioridad comando args\n");
        return;
    }

    int prioridad = atoi(trozos[1]);
    path = Ejecutable(trozos[2], LibroDeBusqueda);

    dataProc newProc;
    newProc.user = strdup(username(getuid()));
    newProc.cmd = strdup(trozos[2]);

    time_t currentTime = time(NULL);
    newProc.date = strdup(ctime(&currentTime));

    if (path == NULL || access(path, X_OK) == -1) {
        newProc.pid = 0; // PID ficticio
        newProc.estado = FINISHED;
        newProc.end = 255; // Código de salida por defecto
        printf("No ejecutado: No such file or directory\n");

        // Establecer prioridad como -1
        if (!insertItemProcList(newProc, listProc)) {
            printf("Error: No se pudo añadir el proceso a la lista\n");
        } else {
            newProc.state = -1; // Prioridad por defecto al fallar
            updateItemProcList(newProc, lastProcList(*listProc), listProc);
        }

        return;
    }

    if ((pid = fork()) == 0) {
        // Proceso hijo: establecer prioridad y ejecutar el comando
        if (setpriority(PRIO_PROCESS, getpid(), prioridad) == -1) {
            perror("No se pudo establecer la prioridad");
            exit(EXIT_FAILURE);
        }

        char *args[] = {path, NULL};
        execve(path, args, environ);
        perror("No ejecutado");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Proceso padre: esperar y registrar
        newProc.pid = pid;
        newProc.estado = ACTIVE;
        newProc.end = 0; // Código de salida no definido aún

        if (!insertItemProcList(newProc, listProc)) {
            printf("Error: No se pudo añadir el proceso a la lista\n");
        } else {
            printf("Proceso %d añadido en background con prioridad %d\n", pid, prioridad);
            // Esperar al proceso y registrar salida
            waitpid(pid, &newProc.end, 0);
            newProc.estado = FINISHED;

            // Mostrar directamente la salida del comando
            printf("\n");
            fflush(stdout);
        }
        updateItemProcList(newProc, lastProcList(*listProc), listProc);
    } else {
        perror("Error al crear proceso en background");
    }
}

void listar(tListProc *listProc){
	int pos;
	dataProc item;
	if(isEmptyProcList(*listProc)){
		return;
	} else {
		for(pos = firstProcList(*listProc); pos <= lastProcList(*listProc); pos++ ){
			item = getItemProcList(pos, *listProc);
			item = actualizar_estado(item, 1);
			updateItemProcList(item, pos, listProc);

      char *estado;
      if(item.estado == STOPPED){
        estado = "STOPPED";
        printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.cmd);
      }
      if(item.estado == ACTIVE){
        estado = "ACTIVE";
        printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.cmd);
      }
      if(item.estado == SIGNALED){
        estado = "SIGNALED";
        printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.cmd);
      }
      if(item.estado == FINISHED){
        estado = "FINISHED";
        printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.cmd);
      }
      if(item.estado == UNKNOWN){
        estado = "UNKOWN";
      }
		}
	}
}

void listjobs (char *trozos[], tListProc *listProc){
	if(trozos[1]==NULL){
    listar(listProc);
	}
	else {
		printf("Error: Número de argumentos inválido\n");
	}
}

dataProc actualizar_estado(dataProc item, int opciones){
	int estado;
	if(opciones != 0){
		opciones = WNOHANG | WUNTRACED | WCONTINUED; 
	}

	if(waitpid(item.pid, &estado, opciones)==item.pid){
		if(WIFEXITED(estado)){
			item.state = FINISHED;
			item.estado = FINISHED;
			item.end = WEXITSTATUS(estado);
		} else if (WIFCONTINUED(estado)){
			item.state = ACTIVE;
			item.estado = ACTIVE;
		} else if (WIFSTOPPED(estado)){
			item.state = STOPPED;
			item.estado = STOPPED;
			item.end = WTERMSIG(estado);
		} else if (WIFSIGNALED(estado)){
			item.state = SIGNALED;
			item.estado = SIGNALED;
			item.end = WTERMSIG(estado);
		} else {
			item.state = UNKNOWN;
			item.estado = UNKNOWN;
		}
	}
	return item;
}

void deljobs(char *trozos[], tListProc *listProc) {
  if(trozos[1]==NULL){
		return;
	} else if(strcmp(trozos[1], "-term")==0 && trozos[2]==NULL){
    int pos;
    dataProc item;
    if(isEmptyProcList(*listProc)){
      return;
    } else {
      for(pos = firstProcList(*listProc); pos<=lastProcList(*listProc); pos++){
        item = getItemProcList(pos, *listProc);
        if(item.estado==FINISHED){
          deleteItemProcList(pos, listProc);
        }
      }
    }
		return;
	} else if (strcmp(trozos[1], "-sig")==0 && trozos[2]==NULL){
		int pos;
    dataProc item;
    if(isEmptyProcList(*listProc)){
      return;
    } else {
      for(pos = firstProcList(*listProc); pos<=lastProcList(*listProc); pos++){
        item = getItemProcList(pos, *listProc);
        if(item.estado==SIGNALED){
          deleteItemProcList(pos, listProc);
        }
      }
    }
		return;
	} else {
		listar(listProc);
		return;
	}
}

void search(char *trozos[], tSearchList *searchList, tSearchList *LibroDeBusqueda) {
    if (trozos[1] == NULL) {
        // Sin argumentos, imprimir la lista actual
        if (isEmptySearchList(*searchList)) {
            printf("Lista de búsqueda vacía.\n");
        } else {
            printf("Lista de búsqueda:\n");
            for (int i = firstSearchList(*searchList); i < lastSearchList(*searchList); i = nextSearchList(*searchList, i)) {
                printf("%s\n", getItemSearchList(*searchList, i));
            }
        }
        return;
    }

    if (!strcmp(trozos[1], "-add")) {
        if (trozos[2] == NULL) {
            printf("Error: Debe especificar un directorio.\n");
            return;
        }
        if (insertSearchList(searchList, trozos[2])) {
            printf("Directorio %s añadido a la lista de búsqueda.\n", trozos[2]);
        } else {
            printf("Error al añadir el directorio.\n");
        }
    } else if (!strcmp(trozos[1], "-del")) {
        if (trozos[2] == NULL) {
            printf("Error: Debe especificar un directorio.\n");
            return;
        }
        removeSearchList(searchList, trozos[2]);
        printf("Directorio %s eliminado de la lista de búsqueda.\n", trozos[2]);
    } else if (!strcmp(trozos[1], "-clear")) {
        createEmptySearchList(searchList);
        printf("Lista de búsqueda limpiada.\n");
    } else if (!strcmp(trozos[1], "-path")) {
        char *path = getenv("PATH");
        int count = 0;
        if (path) {
            char *dir = strtok(path, ":");
            while (dir != NULL) {
                if (insertSearchList(searchList, dir)) {
                    count++;
                }
                dir = strtok(NULL, ":");
            }
            // Sincronizar con la lista global
            LibroDeBusqueda = searchList;
            printf("Importados %d directorios en la ruta de búsqueda.\n", count);
        } else {
            printf("Error al acceder a PATH.\n");
        }
    } else {
        printf("Comando no reconocido para search.\n");
    }
}



void fg(char *trozos[], tListProc *listProc) {
    pid_t pid;

    if (trozos[1] == NULL) {
        printf("Uso: fg PID\n");
        return;
    }

    pid = (pid_t)atoi(trozos[1]);
    int i;
    dataProc proc;

    // Buscar el proceso en la lista de procesos
    for (i = firstProcList(*listProc); i <= lastProcList(*listProc); i++) {
        proc = getItemProcList(i, *listProc);
        if (proc.pid == pid) {
            // Esperar al proceso en foreground
            if (waitpid(pid, NULL, 0) == -1) {
                perror("Error esperando al proceso");
            } else {
                printf("Proceso %d ejecutado en foreground completado.\n", pid);
                deleteItemProcList(i, listProc);
            }
            return;
        }
    }

    printf("Error: No se encontró el proceso con PID %d en la lista de procesos en background.\n", pid);
}

void fgpri(char *trozos[], tListProc *listProc) {
    if (trozos[1] == NULL || trozos[2] == NULL) {
        printf("Uso: fgpri prio PID\n");
        return;
    }

    int prioridad = atoi(trozos[1]);
    pid_t pid = (pid_t)atoi(trozos[2]);
    int i;
    dataProc proc;

    for (i = firstProcList(*listProc); i <= lastProcList(*listProc); i++) {
        proc = getItemProcList(i, *listProc);
        if (proc.pid == pid) {
            printf("Cambiando prioridad del proceso %d a %d...\n", pid, prioridad);
            if (setpriority(PRIO_PROCESS, pid, prioridad) == -1) {
                perror("Error al cambiar la prioridad");
                return;
            }
            printf("Trayendo proceso %d al foreground...\n", pid);
            if (waitpid(pid, NULL, 0) == -1) {
                perror("Error esperando al proceso");
            } else {
                printf("Proceso %d completado.\n", pid);
                deleteItemProcList(i, listProc);
            }
            return;
        }
    }

    printf("Error: No se encontró el proceso con PID %d.\n", pid);
}

int cambiarPrioridad(char *val, pid_t pid) {
	int pri;
	pri = strtol(val, NULL, 10);
	if(setpriority(PRIO_PROCESS, pid, pri)==-1){
		return -1;
	}	
	return pri;
}