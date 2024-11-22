

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

//FUNCION PARA IMPRIMIR LA LISTA DE MEMORIA
void printMemoryList(tListM mL) {
    int p;
    dataMem itm;
    printf("******Lista de bloques asignados para el proceso %d\n", getpid());

    if(isEmptyMemList(mL)){
        printf("\n");
    } else {
        for(p = firstMemListPos(mL); p <= lastMemListPos(mL); p++) {
            itm = getDataItemList(mL, p);
            if(itm.cmdType == MALLOC)
                printf("\t%p\t\t%zu %s malloc\n", itm.dir, itm.size, itm.date);

            if(itm.cmdType == SHARED)
                printf("\t%p\t\t%zu %s shared (key: %d)\n", itm.dir, itm.size, itm.date, itm.key);

            if(itm.cmdType == MMAP)
                printf("\t%p\t\t%zu %s %s  (descriptor %d)\n", itm.dir, itm.size, itm.date, itm.fichero.filename, itm.fichero.df);
        }
    }
}


void aux_malloc(char *trozos[], tListM *mL) {
    size_t tam;
    dataMem item;
    time_t tiempo = time(0);
    struct tm * tlocal = localtime(&tiempo);

    tam = strtol(trozos[2], NULL, 10);

    if(tam == 0)
        printf("No se asignan bloques de 0 bytes\n");
    else {
        item.cmdType = MALLOC;
        item.size = tam;
        item.dir = malloc(tam);
        strftime(item.date, 128, "%b %d %H:%M", tlocal);

        if(item.dir == NULL) {
            perror("No se pudo conseguir la direccion.");
            return;
        } else {
            if(insertMemListPos(mL, item)){
                printf("Asignados %zu bytes en %p\n", item.size, item.dir);
            }else
                perror("Imposible hacer malloc.");
        }
    }
}



void allocate(char *tr[], tListM *L) {
    if (tr[1] == NULL || tr[2] == NULL) {
        printMemoryList(*L);
        return;
    }

    // Caso de malloc
    if (strcmp(tr[1], "-malloc") == 0 && tr[2] != NULL) {
        aux_malloc(tr, L);  // Llamada a la función que maneja malloc
    }
    // Caso de mmap
    else if (strcmp(tr[1], "-mmap") == 0 && tr[2] != NULL) {
        do_AllocateMmap(tr, L);
    }
    // Caso de memoria compartida -creada
    else if (strcmp(tr[1], "-createshared") == 0 && tr[2] != NULL && tr[3] != NULL) {
        do_AllocateCreateshared(tr + 2, *L);
    }
    // Caso de memoria compartida
    else if (strcmp(tr[1], "-shared") == 0 && tr[2] != NULL) {
        key_t clave = (key_t)strtoul(tr[2], NULL, 10);  // Convierte el argumento a clave
        if (clave == 0) {
            printf("Clave inválida para memoria compartida.\n");
            return;
        }
        do_AllocateShared(tr + 2, L);  // Llama a la función correspondiente
    }
    else {
        printf("Comando no reconocido o parámetros incorrectos.\n");
    }
}


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
    }
    else {
        Recursiva(atoi(trozos[1]));
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

void memoryGen(char *trozos[], tListM *mL) {
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
        printMemoryList(*mL);
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
        printMemoryList(*mL);
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
    p=cadtop(trozos[2]);  /*convertimos de cadena a puntero*/
    if (trozos[3]!=NULL)
        cont=(size_t) atoll(trozos[3]);

    if ((n=LeerFichero(trozos[1],p,cont))==-1)
        perror ("Imposible leer fichero\n");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,trozos[1],p);
}

void readC(char *trozos[]){
    void *p;
    size_t cont;
    ssize_t n;
    int df;
    if (trozos[1]==NULL || trozos[2]==NULL || trozos[3]==NULL){
        perror("Faltan parametros\n");
        return;
    }
    df=atoi(trozos[1]);

    if(df<0){
        perror("EL fichero no está abierto\n");
        return;
    }
    p=cadtop(trozos[2]);

    if (trozos[3]!=NULL)
        cont=(size_t) atoll(trozos[3]);
    else
        cont=-1;

    // conseguir la lista de ficheros,
    // recorrerla comparando cada df de cada posición al df que me dan
    // una vez lo tengo, consigo el nombre
    // y en LeerFichero mando el nombre en vez de trozos1

    if((n=LeerFichero(trozos[1], p, cont))==-1) //
        perror("Imposible leer desde el fichero");
    else
        printf("Leídos %lld bytes desde el descriptor de archivo %d en %p\n", (long long) n, df, p);

}


//función auxiliar para memfill
void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
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
    LlenarMemoria(p,nbytes,byte);
}


//funcion auxiliar para memdump

void dumpMem(const char *p, int nbytes) {
    int i, j;
    unsigned char c;

    for(i = 0; i < nbytes; i += 25) {
        for (j = i; j < i + 25 && j < nbytes; j++) {
            c = *(p + j);

            if (c >= 32 && c <= 127)
                printf("%c ", c);
            else
                printf(" ");
        }
        printf("\n");

        for (j = i; j < i + 25 && j < nbytes; j++) {
            c = *(p + j);
            printf("%02X ", c);
        }
        printf("\n");
    }
}



void memdump(char *trozos[]) {
    char *p = NULL;
    int nbytes = 25;

    if (trozos[1] == NULL) {
        printf("Faltan parámetros\n");
        return;
    }

    p = (char *)strtoul(trozos[1], NULL, 16);
    if (p == NULL) {
        perror("Dirección no válida");
        return;
    }

    if (trozos[2] != NULL) {
        nbytes = atoi(trozos[2]);
        if (nbytes <= 0) {
            printf("El número de bytes tiene que ser mayor que 0.\n");
            return;
        }
    }

    printf("Volcando %d bytes desde la dirección %p\n", nbytes, p);
    dumpMem(p, nbytes);
}

void deallocate(char *tr[], tListM *L) {
    if (tr[1] == NULL) {
        printf("Debe especificar el tipo de memoria a liberar.\n");
        return;
    }

    if (strcmp(tr[1], "-addr") == 0) {
        // Asegurarse de que la dirección se pasa correctamente en formato hexadecimal
        void *addr = (void *)strtoull(tr[2], NULL, 16);  // Convertimos la dirección hexadecimal (ejemplo: 0x12345)

        if (addr == NULL) {
            printf("Dirección inválida para deallocar.\n");
            return;
        }

        printf("Intentando liberar la dirección: %p\n", addr);

        // Llamamos a la función para deallocar la dirección específica
        deallocate_addr(addr, L);
        return;
    }

    if (strcmp(tr[1], "-malloc") == 0 && tr[2] != NULL) {
        size_t size = (size_t)strtoul(tr[2], NULL, 10);
        if (size == 0) {
            printf("No se puede liberar memoria de tamaño 0.\n");
            return;
        }

        do_DeallocateMalloc(size, L);  // Llama a la función para liberar la memoria malloc
    }
    else if (strcmp(tr[1], "-mmap") == 0 && tr[2] != NULL) {
        char *file = tr[2];  // El nombre del archivo está en tr[2]
        do_DeallocateMmap(file, L);  // Llama a la función para liberar el mapeo del archivo
    }
    else if (strcmp(tr[1], "-shared") == 0 && tr[2] != NULL) {
        char *key_str = tr[2];
        key_t clave = (key_t)strtoul(key_str, NULL, 10);  // Convierte la clave de memoria compartida
        if (clave == IPC_PRIVATE) {
            printf("Clave inválida para memoria compartida.\n");
            return;
        }

        // Llamar a la función que elimina la memoria compartida
        do_DeallocateDelkey(tr+2);
    }
    else {
        printf("Comando no reconocido o parámetros incorrectos.\n");
    }
}



void InsertarNodoShared(tListM *memList, void *dir, size_t tam, key_t clave) {
    // Crear un nuevo bloque de datos de tipo SHARED
    dataMem nuevoBloque;

    // Inicializar los campos del nuevo bloque
    nuevoBloque.size = tam;
    nuevoBloque.dir = dir;
    nuevoBloque.cmdType = SHARED;
    nuevoBloque.key = clave;

    // Insertar el nuevo bloque en la lista
    if (!insertMemListPos(memList, nuevoBloque)) {
        printf("Error al insertar el bloque de memoria compartida.\n");
    }
}


void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListM L) {
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

void do_AllocateCreateshared (char *tr[], tListM L) { //Crea un bloque de memoria compartida
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
    if ((p=ObtenerMemoriaShmget(cl,tam,L))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void ImprimirListaShared(tListM *memList) {
    int i;
    dataMem itm;

    // Si la lista está vacía
    if (isEmptyMemList(*memList)) {
        printf("La lista de bloques de memoria compartida está vacía.\n");
        return;
    }

    printf("Lista de bloques de memoria compartida:\n");
    printf("%-20s %-20s %-10s %-10s\n", "Clave", "Dirección", "Tamaño", "Fecha");
    printf("------------------------------------------------------------\n");

    // Recorremos todos los bloques de memoria en la lista
    for (i = firstMemListPos(*memList); i <= lastMemListPos(*memList); i++) {
        itm = getDataItemList(*memList, i);

        // Verifica si el bloque de memoria es de tipo SHARED
        if (itm.cmdType == SHARED) {
            // Imprime la información del bloque de memoria compartida
            printf("%-20d %-20p %-10zu %-10s\n", itm.key, itm.dir, itm.size, itm.date);
        }
    }
}

void do_AllocateShared (char *tr[], tListM *L)
{
    key_t cl;
    //size_t tam;
    void *p;

    if (tr[0]==NULL) {
        ImprimirListaShared(L);
        return;
    }

    cl=(key_t)  strtoul(tr[0],NULL,10);

    if ((p=ObtenerMemoriaShmget(cl,0,*L))!=NULL)
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

void ImprimirListaMmap(tListM *memList) {
    int i;
    dataMem itm;

    // Si la lista está vacía
    if (isEmptyMemList(*memList)) {
        printf("La lista de memoria está vacía.\n");
        return;
    }

    printf("Lista de ficheros mapeados:\n");
    printf("%-30s %-20s %-10s\n", "Nombre del fichero", "Dirección", "Tamaño");
    printf("------------------------------------------------------------\n");

    // Recorremos todos los bloques de memoria en la lista
    for (i = firstMemListPos(*memList); i <= lastMemListPos(*memList); i++) {
        itm = getDataItemList(*memList, i);

        // Verifica si el bloque de memoria es de tipo MMAP
        if (itm.cmdType == MMAP) {
            // Imprime la información del bloque mapeado
            printf("%-30s %-20p %-10zu\n", itm.fichero.filename, itm.dir, itm.size);
        }
    }
}

void do_AllocateMmap(char *arg[], tListM *L){//funcion para hacer un mapeado de un fichero
    char *perm;
    void *p;
    int protection=0;
    dataMem item;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    struct stat file;
    stat(arg[2], &file);

    if (arg[2]==NULL)
        {ImprimirListaMmap(L); return;}
    if ((perm=arg[3])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MapearFichero(arg[2],protection))==NULL)
        perror ("Imposible mapear fichero");
    else{
        item.cmdType = MMAP;
        item.size = file.st_size;
        item.dir = p; 
        strftime(item.date, 128, "%b %d %H:%M", tlocal);
        item.fichero.df = (int)open(arg[2], O_CREAT, O_RDONLY);
        strcpy(item.fichero.filename,arg[2]);
        if(insertMemListPos(L,item)){
            printf ("fichero %s mapeado en %p\n", arg[2], p);
        } else {
            printf("No ha sido posible mapear el archivo\n");
        }
    }
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

void do_DeallocateMalloc(size_t size, tListM *L) {
    int aux;
    for (aux = 0; aux <= L->lastPos; aux++) {
        if (L->itemM[aux].cmdType == MALLOC && L->itemM[aux].size == size) {
            free(L->itemM[aux].dir);
            deleteItemMemList(aux, L);
            printf("Memoria malloc de tamaño %zu liberada.\n", size);
            return;
        }
    }
    printf("No se encontró memoria malloc de tamaño %zu.\n", size);
}

void do_DeallocateMmap(char *file, tListM *L) {
    int aux;
    for (aux = 0; aux <= L->lastPos; aux++) {
        if (L->itemM[aux].cmdType == MMAP && strcmp(L->itemM[aux].fichero.filename, file) == 0) {
            munmap(L->itemM[aux].dir, L->itemM[aux].size);
            close(L->itemM[aux].fichero.df);  // Cierra el descriptor del archivo
            deleteItemMemList(aux, L);             // Elimina el nodo de la lista
            printf("Memoria mapeada para el archivo %s liberada.\n", file);
            return;
        }
    }
    printf("No se encontró mapeo para el archivo %s.\n", file);
}

void deallocate_addr(void *addr, tListM *L) {
    int aux;
    for (aux = 0; aux <= L->lastPos; aux++) {
        // Comprobamos si el bloque de memoria coincide con la dirección proporcionada
        if (L->itemM[aux].dir == addr) {
            // Si es un bloque malloc, lo liberamos
            if (L->itemM[aux].cmdType == MALLOC) {
                free(L->itemM[aux].dir);
                deleteItemMemList(aux, L);  // Eliminar el bloque de la lista
                printf("Memoria malloc con dirección %p liberada.\n", addr);
            }
            // Si es un bloque mmap, lo desconectamos
            else if (L->itemM[aux].cmdType == MMAP) {
                munmap(L->itemM[aux].dir, L->itemM[aux].size);
                close(L->itemM[aux].fichero.df);  // Cierra el descriptor del archivo
                deleteItemMemList(aux, L);             // Elimina el nodo de la lista
                printf("Memoria mmap con dirección %p liberada.\n", addr);
            }
            return;
        }
    }

    printf("No se encontró un bloque de memoria con la dirección %p.\n", addr);
}