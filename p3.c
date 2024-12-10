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

/*void environ(char *trozos[], char * arg3[], char *envi[]){

}*/


void Cfork(char * trozos[], tListProc * ListProc){
  pid_t pid;
  if(pid!=-1)
    waitpid(pid,NULL,0);
  else if((pid = fork()) == 0){
    deleteProcList(ListProc);
    printf("Ejecutando processo %d \n", getpid());
}
}