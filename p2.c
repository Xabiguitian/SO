//
// Created by angela on 15/11/24.
//

//LIBRERIAS DEFINIDAS
#include "p1.h"
#include "p0.h"
#include "p2.h"
#include "historial.h"
#include "file.h"
#include "memlist.h"
#define TAMANO 2048

tListM L;

void Recursiva(int n) {
   char automatico[TAMANO];
   static char estatico[TAMANO];

   printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

   if (n > 0)
      Recursiva(n - 1);
}

void LlenarMemoria(void *p, size_t cont, unsigned char byte) {
   unsigned char *arr = (unsigned char *)p;
   size_t i;

   for (i = 0; i < cont; i++)
      arr[i] = byte;
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam) {
   void *p;
   int aux, id, flags = 0777;
   struct shmid_ds s;

   if (tam)
      flags = flags | IPC_CREAT | IPC_EXCL;
   if (clave == IPC_PRIVATE) {
      errno = EINVAL;
      return NULL;
   }
   if ((id = shmget(clave, tam, flags)) == -1)
      return NULL;
   if ((p = shmat(id, NULL, 0)) == (void *)-1) {
      aux = errno;
      if (tam)
         shmctl(id, IPC_RMID, NULL);
      errno = aux;
      return NULL;
   }
   shmctl(id, IPC_STAT, &s);
   InsertarNodoShared(&L, p, s.shm_segsz, clave);
   return p;
}

void do_AllocateCreateshared (char *tr[]) { //Crea un bloque de memoria compartida
   key_t cl;
   size_t tam;
   void *p;

   if (tr[0]==NULL || tr[1]==NULL) {
		ImprimirListaShared(&L);
		return;
   }

   cl=(key_t)  strtoul(tr[0],NULL,10);
   tam=(size_t) strtoul(tr[1],NULL,10);
   if (tam==0) {
	printf ("No se asignan bloques de 0 bytes\n");
	return;
   }
   if ((p=ObtenerMemoriaShmget(cl,tam))!=NULL)
		printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
   else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void ImprimirListaShared(tListM  * memList) {
   int aux;
   dataMem itm;

   printf("LISTA DE BLOQUES DE MEMORIA COMPARTIDA:\n");
   if (isEmptyMemList(*memList)) {
      printf("\b");
   } else {
      for (aux = firstMem(*memList); aux <= lastMem(*memList); aux++) {
         itm = getDataMemList(aux, *memList);
         if (itm.cmdType == SHARED)
            printf("\t%p\t\t%zu %s shared (key: %d)\n", itm.dir, itm.size, itm.date, itm.Union.key);
      }
   }
}

void do_AllocateShared (char *tr[])
{
   key_t cl;
   size_t tam;
   void *p;

   if (tr[0]==NULL) {
		ImprimirListaShared(&L);
		return;
   }

   cl=(key_t)  strtoul(tr[0],NULL,10);

   if ((p=ObtenerMemoriaShmget(cl,0))!=NULL)
		printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
   else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void * MapearFichero (char * fichero, int protection){ //mapea un fichero
   int df, map=MAP_PRIVATE,modo=O_RDONLY;
   struct stat s;
   void *p;

   if (protection&PROT_WRITE)
      modo=O_RDWR;
   if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
      return NULL;
   if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
      return NULL;
/* Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
/* Gurdas en la lista de descriptores usados df, fichero*/
   return p;
}

void ImprimirListaMmap(tListM *memList){
   int pos;
   dataMem itm;
   printf("LISTA DE BLOQUES ASIGNADOS PARA EL PROCESO: ½d \n",getpid());
   if(isEmptyMemList(*memList))
      printf("\b");
   else{
      for(pos= firstMem(*memList);pos<=lastMem(*memList);pos++){
         itm=getDataMemList(pos, *memList);
         if(itm.cmdType==MMAP)
            printf("\t%p\t\t%zu %s %s (descriptor %d)\n", itm.dir, itm.size, itm.date, itm.Union.fichero.name, itm.Union.fichero.id);
      }
   }
}

void do_AllocateMmap(char *arg[]){//funcion para hacer un mapeado de un fichero
   char *perm;
   void *p;
   int protection=0;

   if (arg[0]==NULL)
      {ImprimirListaMmap(&L); return;}
   if ((perm=arg[1])!=NULL && strlen(perm)<4) {
      if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
      if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
      if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
   }
   if ((p=MapearFichero(arg[0],protection))==NULL)
      perror ("Imposible mapear fichero");
   else
      printf ("fichero %s mapeado en %p\n", arg[0], p);
}

void do_DeallocateDelkey (char *args[]){ //función para borrar una clave de un bloque de memoria compartida
   key_t clave;
   int id;
   char *key=args[0];

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
      printf ("      delkey necesita clave_valida\n");
      return;
   }
   if ((id=shmget(clave,0,0666))==-1){
      perror ("shmget: imposible obtener memoria compartida");
      return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
      perror ("shmctl: imposible eliminar memoria compartida\n");
}


ssize_t LeerFichero (char *f, void *p, size_t cont) { //funcion para leer un fichero
   struct stat s;
   ssize_t  n;
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}

void * cadtop(char *s){
   void *p;
   sscanf(s, "%p", &p);
   return p;

}

void Cmd_ReadFile (char *ar[])
{
   void *p;
   size_t cont=-1;  /*si no pasamos tamano se lee entero */
   ssize_t n;
   if (ar[0]==NULL || ar[1]==NULL){
	printf ("faltan parametros\n");
	return;
   }
   p=cadtop(ar[1]);  /*convertimos de cadena a puntero*/
   if (ar[2]!=NULL)
	cont=(size_t) atoll(ar[2]);

   if ((n=LeerFichero(ar[0],p,cont))==-1)
	perror ("Imposible leer fichero");
   else
	printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

void Do_pmap (void) /*sin argumentos*/
{ pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};

   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible crear proceso");
      return;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");

      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");

      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");

      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");
      exit(1);
   }
   waitpid (pid,NULL,0);
}

void do_DeallocateMalloc(size_t size) {
   int aux;
   for (aux = 0; aux <= L.lastPos; aux++) {
      if (L.itemM[aux].cmdType == MALLOC && L.itemM[aux].size == size) {
         free(L.itemM[aux].dir);
         deleteItemMemList(aux, &L);
         printf("Memoria malloc de tamaño %zu liberada.\n", size);
         return;
      }
   }
   printf("No se encontró memoria malloc de tamaño %zu.\n", size);
}

void do_DeallocateMmap(char *file) {
   int aux;
   for (aux = 0; aux <= L.lastPos; aux++) {
      if (L.itemM[aux].cmdType == MMAP && strcmp(L.itemM[aux].Union.fichero.name, file) == 0) {
         munmap(L.itemM[aux].dir, L.itemM[aux].size);
         close(L.itemM[aux].Union.fichero.id);  // Cierra el descriptor del archivo
         deleteItemMemList(aux, &L);             // Elimina el nodo de la lista
         printf("Memoria mapeada para el archivo %s liberada.\n", file);
         return;
      }
   }
   printf("No se encontró mapeo para el archivo %s.\n", file);
}

void do_Deallocate(char *trozos[]){
   DIR *dir;
   struct dirent *entry;
   struct stat fileStat;
   char *direccion = ".";

   if (trozos[1] == NULL) {
      cwd();
      return;
   } else {
      direccion = trozos[1];
      for (int i = 1; trozos[i] != NULL; i++) {
         if (strcmp(trozos[i], "-malloc") == 0) do_DeallocateMalloc();
         else if (strcmp(trozos[i], "-mmap") == 0) do_DeallocateMmap();
         else if (strcmp(trozos[i], "-shared") == 0) ;
         else if (strcmp(trozos[i], "-delkey") == 0) do_DeallocateDelkey();
         else ;
      }
   }
}