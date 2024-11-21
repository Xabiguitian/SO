

//LIBRERIAS DEFINIDAS
#include "p1.h"
#include "p0.h"
#include "p2.h"
#include "historial.h"
#include "file.h"
#include "memlist.h"
#define TAMANO 2048

//
int intGlobI = 4;
double doubleGlobI = 2.5;
char charGlobI = 'z';


// Variables globales sin inicializar:
int intGlobSN;
double doubleGlobSN;
char charGlobSN;


/*void deallocateGen();
void allocateGen();
void memfill();
void memdump();
void memoryGen();
void readfile();
void writefile();
void writeC();
void readC();
 */



//FUNCION AUXILIAR PARA RECURSE
void Recursiva (int n){
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

//COMANDO RECURSE
void recurse(char *trozos[]){
    if (trozos[1]==NULL){
        printf("recurse [n]\t Invoca a la funcion recursiva n veces\n");
    }else {
        Recursiva(atoi(trozos[1]));
    }
}
//FUNCION PARA IMPRIMIR LA LISTA DE MEMORIA
void printMemoryList(tListM mL) {
    int p;
    dataMem itm;

    printf("******Lista de bloques asignados para el proceso %d\n", getpid());

    if(isEmptyMemList(mL))
        printf("\b");
    else {
        for(p = firstMemListPos(mL); p <= lastMemListPos(mL); p++) {
            itm = getDataItemList(mL, p);

            if(itm.cmdType == MALLOC)
                printf("\t%p\t\t%zu %s malloc\n", itm.dir, itm.size, itm.date);

            if(itm.cmdType == SHARED)
                printf("\t%p\t\t%zu %s shared (key: %d)\n", itm.dir, itm.size, itm.date, itm.Union.key);

            if(itm.cmdType == MMAP)
                printf("\t%p\t\t%zu %s %s  (descriptor %d)\n", itm.dir, itm.size, itm.date, itm.Union.fichero.filename, itm.Union.fichero.df);
        }
    }
}

//FUNCIÓN PARA MEMORY -PMAP
void pMap(void){
    id_t pid;
    char tpid[32];
    char *argv[4]={"pmap",tpid,NULL};

    sprintf (tpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=tpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=tpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=tpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid (pid,NULL,0);
}

void memoryGen(char *trozos[], tListM mL) {
    int static a;
    double static b;
    char static c;
    int static d=4;
    double static e=2.5;
    char static f='f';
    int auto1;
    double auto2;
    char auto3;


    if(trozos[1]==NULL||strcmp(trozos[1], "-all")==0){
        printf("Funciones programa:\t\t %p, \t\t%p, \t\t %p\n", &pid,&cmdate,&authors);
        printf("Funciones libreria:\t\t %p, \t\t%p, \t\t %p\n", &malloc, &printf,&scanf);
        printf("Variables globales:\t\t %p\n %p\n %p\n",&intGlobI,&doubleGlobI,&doubleGlobI);
        printf("Variables (N.I.)globales:\t\t %p,\t\t%p, \t\t%p\n",&intGlobSN,&doubleGlobSN,&doubleGlobSN );
        printf("Variables estáticas:\t\t %p\n %p\n %p\n", &d,&e,&f);
        printf("Variables (N.I)estáticas:\t\t %p\n %p\n %p\n",&a,&b,&c);
        printf("Variables locales:\t\t %p\n %p\n %p\n",&auto1,&auto2,&auto3);
        printMemoryList(mL);
    }else if(strcmp(trozos[1],"-funcs")==0){
        printf("Funciones programa:\t\t %p, \t\t%p, \t\t %p\n", &pid,&cmdate,&authors);
        printf("Funciones libreria:\t\t %p, \t\t%p, \t\t %p\n", &malloc, &printf,&scanf);
    }else if(strcmp(trozos[1],"-vars")==0){
        printf("Variables globales:\t\t %p\n %p\n %p\n",&intGlobI,&doubleGlobI,&doubleGlobI);
        printf("Variables (N.I.)globales:\t\t %p,\t\t%p, \t\t%p\n",&intGlobSN,&doubleGlobSN,&doubleGlobSN );
        printf("Variables estáticas:\t\t %p\n %p\n %p\n", &d,&e,&f);
        printf("Variables (N.I)estáticas:\t\t %p\n %p\n %p\n",&a,&b,&c);
        printf("Variables locales:\t\t %p\n %p\n %p\n",&auto1,&auto2,&auto3);
    }else if(strcmp(trozos[1],"-blocks")==0){
        printMemoryList(mL);
    }else if(strcmp(trozos[1],"-pmap")==0){
        pMap();
    }
}



void writefile(char *trozos[]) {
    void *direccion;
    char sobrescribir=0;
    int cantB, df;

    if (trozos[1]==NULL){
        printf("Faltan parámetros.\n");
        return;
    }

    if(strcmp("-o", trozos[1])!=0){
        sobrescribir=1;
    }else if(trozos[1][0]=='-'){
        printf("Opcion no reconocida.\n");
        return;
    }

    if(trozos[1+sobrescribir]==NULL){
        perror("Archivo no especificado");
        return;
    }
    if(trozos[2+sobrescribir]==NULL){
        perror("Dirección no especificada");
        return;
    }
    if(trozos[3+sobrescribir]==NULL){
        perror("Cantidad no especificada");
        return;
    }
    direccion=(void *) strtoul(trozos[2+sobrescribir], NULL, 16);
    cantB=atoi(trozos[3+sobrescribir]);

    if(sobrescribir==1){
        df= open(trozos[1+sobrescribir], O_CREAT |O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }else{
        df=open(trozos[1 + sobrescribir], O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }

    if(df==-1){
        perror("Error al abrir el archivo");
        return;
    }

    if(write(df, direccion, cantB)==-1){
        close(df);
        perror("Error al escribir el archivo");
        return;
    }else{
        printf("Escritos %d bytes desde %p en el archivo\n",cantB,direccion);
        close(df);
    }

    }


void writeC(char *trozos[]){
    int  df;
    if(trozos[1]==NULL){
        perror("Descriptor de archivo no especificado");
    }
    df=atoi(trozos[1]);
    if(df<0){
        perror("No está abierto");
        return;
    }
    writefile(trozos);
}



void * cadtop(char *s){
    void *p;
    sscanf(s, "%p", &p);
    return p;
}

ssize_t LeerFichero (char *f, void *p, size_t cont)
{
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

void Cmd_ReadFile (char *trozos[])
{
    void *p;
    size_t cont=-1;  /*si no pasamos tamano se lee entero */
    ssize_t n;
    if (trozos[0]==NULL || trozos[1]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(trozos[1]);  /*convertimos de cadena a puntero*/
    if (trozos[2]!=NULL)
        cont=(size_t) atoll(trozos[2]);

    if ((n=LeerFichero(trozos[0],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,trozos[0],p);
}

void readC(char *trozos[]){
    void *p;
    size_t cont;
    ssize_t n;
    int df;
    if (trozos[0]==NULL || trozos[1]==NULL){
        perror("Faltan parametros\n");
        return;
    }
    df=atoi(trozos[0]);

    if(df<0){
        perror("EL fichero no está abierto\n");
        return;
    }
    p=cadtop(trozos[1]);

    if (trozos[2]!=NULL)
        cont=(size_t) atoll(trozos[2]);
    else
        cont=-1;

    if((n=LeerFichero(df,p,cont))==-1)
        perror("Imposible leer desde el fichero");
    else
        printf("Leídos %lld bytes desde el descriptor de archivo %d en %p\n", (long long) n, df, p);

}


//función auxiliar para memfill
void fillMem(void *p, size_t size, unsigned char value){
    unsigned char *pt=(unsigned char *)p;
    size_t aux;

    for(aux=0;aux<size;++aux){
        *(pt+aux)=value;
    }
}

void memfill(char *trozos[]){
    void *p=NULL;
    size_t nbytes=128;
    unsigned char byte='A';

    if(trozos[1]==NULL){
        printf("Faltan parametros\n");
        return;
    }else{
        p=(void *)strtoul(trozos[1],NULL,16);
    }
    if(trozos[2]!=NULL){
        nbytes=(size_t) atoi(trozos[2]);
    }
    if(trozos[3]!=NULL){
        byte=(unsigned char) trozos[3][0];
    }
    fillMem(p,nbytes,byte);
}

void allocate(char *tr[]) {
    if (tr[0] == NULL) {
        printf("Debe proporcionar un argumento.\n");
        return;
    }

    // Caso 1: "-malloc n" -> Asignar memoria con malloc
    if (strcmp(tr[0], "-malloc") == 0 && tr[1] != NULL) {
        size_t tam = (size_t)strtoul(tr[1], NULL, 10);
        if (tam == 0) {
            printf("No se puede asignar memoria de tamaño 0\n");
            return;
        }

        void *ptr = AsignarMemoriaMalloc(tam);
        if (ptr != NULL) {
            dataMem nuevoBloque = {tam, ptr, "fecha_placeholder", .Union.fichero = -1, .cmdType = 0};
            InsertarBloqueMemoria(nuevoBloque);
            printf("Bloque de memoria de %lu bytes asignado con malloc en %p\n", (unsigned long)tam, ptr);
        } else {
            printf("Error al asignar memoria con malloc\n");
        }
    }
    // Caso 2: "-mmap file perm" -> Asignar memoria mapeando un archivo
    else if (strcmp(tr[0], "-mmap") == 0 && tr[1] != NULL && tr[2] != NULL) {
        char *fichero = tr[1];
        char *perm = tr[2];
        int protection = 0;
        
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;

        void *ptr = AsignarMemoriaMmap(fichero, protection);
        if (ptr != NULL) {
            dataMem nuevoBloque = {0, ptr, "fecha_placeholder", .Union.fichero = open(fichero, O_RDONLY), .cmdType = 1};
            InsertarBloqueMemoria(nuevoBloque);
            printf("Archivo %s mapeado a memoria en %p\n", fichero, ptr);
        } else {
            printf("Error al mapear el archivo %s\n", fichero);
        }
    }
    // Caso 3: "-create shared cl n" -> Asignar memoria compartida
    else if (strcmp(tr[0], "-create") == 0 && tr[1] != NULL && tr[2] != NULL) {
        key_t cl = (key_t)strtoul(tr[1], NULL, 10);
        size_t tam = (size_t)strtoul(tr[2], NULL, 10);
        if (tam == 0) {
            printf("No se pueden asignar bloques de 0 bytes\n");
            return;
        }

        void *ptr = AsignarMemoriaShared(cl, tam);
        if (ptr != NULL) {
            dataMem nuevoBloque = {tam, ptr, "fecha_placeholder", .Union.key = (int)cl, .cmdType = 2};
            InsertarBloqueMemoria(nuevoBloque);
            printf("Memoria compartida con clave %lu asignada en %p\n", (unsigned long)cl, ptr);
        } else {
            printf("Error al asignar memoria compartida con clave %lu\n", (unsigned long)cl);
        }
    }
    // Caso 4: "-shared cl" -> Asignar memoria compartida ya creada
    else if (strcmp(tr[0], "-shared") == 0 && tr[1] != NULL) {
        key_t cl = (key_t)strtoul(tr[1], NULL, 10);
        void *ptr = AsignarMemoriaShared(cl, 0);  // Solo obtención, sin tamaño
        if (ptr != NULL) {
            dataMem nuevoBloque = {0, ptr, "fecha_placeholder", .Union.key = (int)cl, .cmdType = 3};
            InsertarBloqueMemoria(nuevoBloque);
            printf("Memoria compartida con clave %lu asignada en %p\n", (unsigned long)cl, ptr);
        } else {
            printf("Error al obtener memoria compartida con clave %lu\n", (unsigned long)cl);
        }
    }
    // Caso por defecto si no se reconoce el comando
    else {
        printf("Comando no reconocido o parámetros incorrectos.\n");
    }
}


void deallocate(char *tr[]) {
    if (tr[0] == NULL) {
        printf("Debe proporcionar un argumento.\n");
        return;
    }

    // Caso 1: "-malloc n" -> Desasignar memoria malloc
    if (strcmp(tr[0], "-malloc") == 0 && tr[1] != NULL) {
        size_t size = (size_t) strtoul(tr[1], NULL, 10);
        if (size == 0) {
            printf("No se puede liberar memoria de tamaño 0\n");
            return;
        }
        free(tr[1]);  // Liberar el bloque de memoria previamente asignado
        printf("Memoria de %lu bytes liberada con free\n", (unsigned long) size);
    }
    // Caso 2: "-mmap file" -> Desmapear archivo
    else if (strcmp(tr[0], "-mmap") == 0 && tr[1] != NULL) {
        // Asegúrate de que el bloque haya sido mapeado previamente
        if (munmap(tr[1], TAMANO) == -1) {  // Suponiendo que TAMANO es el tamaño de mapeo
            perror("Error al desmapear archivo");
            return;
        }
        printf("Archivo %s desmapeado\n", tr[1]);
    }
    // Caso 3: "-shared cl" -> Desacoplar memoria compartida
    else if (strcmp(tr[0], "-shared") == 0 && tr[1] != NULL) {
        key_t clave = (key_t) strtoul(tr[1], NULL, 10);
        if (shmdt(tr[1]) == -1) {
            perror("Error al desacoplar memoria compartida");
            return;
        }
        printf("Memoria compartida con clave %lu desacoplada\n", (unsigned long) clave);
    }
    // Caso 4: "-delkey cl" -> Eliminar clave de memoria compartida
    else if (strcmp(tr[0], "-delkey") == 0 && tr[1] != NULL) {
        key_t clave = (key_t) strtoul(tr[1], NULL, 10);
        int id;
        if ((id = shmget(clave, 0, 0666)) == -1) {
            perror("shmget: imposible obtener memoria compartida");
            return;
        }
        if (shmctl(id, IPC_RMID, NULL) == -1) {
            perror("shmctl: imposible eliminar memoria compartida");
            return;
        }
        printf("Memoria compartida con clave %lu eliminada\n", (unsigned long) clave);
    }
    // Caso 5: "addr" -> Desasignar memoria según dirección
    else {
        void *addr = (void *) strtoul(tr[0], NULL, 10);
        if (addr == NULL) {
            printf("Dirección no válida\n");
            return;
        }
        
        // Verificar si la dirección es un bloque malloc, mmap o compartido (esto puede ser más complejo dependiendo de la implementación específica)
        // Caso malloc
        // Aquí asumimos que la dirección proporcionada es válida para liberar con free
        free(addr);  // Liberar memoria
        printf("Bloque de memoria en %p liberado\n", addr);
    }
}

/*void allocateGen(char *trozos[]){
    if(trozos[1]==NULL){
        printf("******Lista de bloques asignados para el proceso %d\n", getpid());
    }else if(strcmp(trozos[1],"-malloc")==0){
    }else if(strcmp(trozos[1],"-mmap")==0){
    }else if(strcmp(trozos[1],"-createshared")==0){
    }else if(strcmp(trozos[1],"-shared")==0){
    }
}*/


/*void deallocateGen(char *trozos[]){
    if(trozos[1]==NULL){
        printf("******Lista de bloques asignados para el proceso %d\n", getpid());
    }else if(strcmp(trozos[1],"-malloc")==0){
    }else if(strcmp(trozos[1],"-mmap")==0){
    }else if(strcmp(trozos[1],"-shared")==0){
    }else if(strcmp(trozos[1],"-delkey")==0){
    }else if(strcmp(trozos[1],"addr")==0){
    }

}*/


/*void LlenarMemoria(void *p, size_t cont, unsigned char byte) {
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
    // Guardar en la lista    InsertarNodoMmap (&L,p, s.st_size,df,fichero);
    // Gurdas en la lista de descriptores usados df, fichero
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
    if (cont==-1)   // si pasamos -1 como bytes a leer lo leemos entero
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



void Cmd_ReadFile (char *ar[])
{
    void *p;
    size_t cont=-1;  //si no pasamos tamano se lee entero
    ssize_t n;
    if (ar[0]==NULL || ar[1]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(ar[1]);  //convertimos de cadena a puntero
    if (ar[2]!=NULL)
        cont=(size_t) atoll(ar[2]);

    if ((n=LeerFichero(ar[0],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

void Do_pmap (void) {
    pid_t pid;       //hace el pmap (o equivalente) del proceso actual
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
        if (execvp(argv[0],argv)==-1)//No hay pmap, probamos procstat FreeBSD
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
                if (execvp(argv[0],argv)==-1)  //probamos procmap OpenBSD
            perror("cannot execute procmap (OpenBSD)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) //probamos vmmap Mac-OS
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
}*/

/*void inicializa() {}
void inicializaM() {

}

void inicializaHistorial() {}

void memory_funcs() {
    printf("Función inicializa: %p\n", &inicializa);
    printf("Funcion printf (libreria): %p\n", &printf);
    printf("Funcion malloc (libreria): %p\n", &malloc);
}

void memory_vars() {
    int static
}

void Cmd_memory(char *trozos[]) {


}*/
