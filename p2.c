

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
    struct tm *tlocal = localtime(&tiempo);


    tam = strtol(trozos[2], NULL, 10);

    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
    } else {
        item.cmdType = MALLOC;
        item.size = tam;
        item.dir = malloc(tam);  // Asigna memoria

        if (item.dir == NULL) {
            perror("No se pudo conseguir la direccion.");
            return;
        }

        // Inicializa la memoria asignada a cero para evitar valores no inicializados
        memset(item.dir, 0, tam);

        strftime(item.date, 128, "%b %d %H:%M", tlocal);

        // Inserta el item en la lista de memoria
        if (insertMemListPos(mL, item)) {
            printf("Asignados %zu bytes en %p\n", item.size, item.dir);
        } else {
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
        do_AllocateCreateshared(tr + 2, L);
    }
    // Caso de memoria compartida
    else if (strcmp(tr[1], "-shared") == 0 && tr[2] != NULL) {
        key_t clave = (key_t)strtoul(tr[2], NULL, 10);  // Convierte el argumento a clave
        if (clave == 0) {
            printf("Clave inválida para memoria compartida.\n");
            return;
        }
        do_AllocateShared(tr, L);  // Llama a la función correspondiente
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
    char sobrescribir = 0;
    int cantB, df;

    if (trozos[1] == NULL) {
        printf("Faltan parámetros.\n");
        return;
    }

    // Verifica si se pasó la opción -o para sobrescribir
    if (strcmp("-o", trozos[1]) == 0) {
        sobrescribir = 1;  // Se activa el sobrescribir
    }

    // El archivo debe estar especificado después de la opción (si no es -o)
    if (trozos[1 + sobrescribir] == NULL) {
        perror("Archivo no especificado");
        return;
    }

    // La dirección de memoria debe estar especificada
    if (trozos[2 + sobrescribir] == NULL) {
        perror("Dirección no especificada");
        return;
    }

    // La cantidad de bytes debe estar especificada
    if (trozos[3 + sobrescribir] == NULL) {
        perror("Cantidad no especificada");
        return;
    }

    // Convertir la dirección hexadecimal a puntero
    direccion = (void *)strtoul(trozos[2 + sobrescribir], NULL, 16);
    cantB = atoi(trozos[3 + sobrescribir]);

    // Comprobar si el archivo ya existe y no se pasa la opción -o
    if (!sobrescribir) {
        df = open(trozos[1 + sobrescribir], O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if (df == -1) {
            perror("Imposible escribir fichero: File exists");
            return;
        }
    } else {
        // Si se pasa la opción -o, sobrescribir el archivo
        df = open(trozos[1 + sobrescribir], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }

    // Comprobar si se ha abierto el archivo correctamente
    if (df == -1) {
        perror("Error al abrir el archivo");
        return;
    }

    // Escribir los datos desde la dirección de memoria al archivo
    if (write(df, direccion, cantB) == -1) {
        close(df);
        perror("Error al escribir el archivo");
        return;
    } else {
        printf("Escritos %d bytes desde %p en el archivo\n", cantB, direccion);
        close(df);
    }
}

void writeC(char *trozos[]) {
    int df;
    if (trozos[1] == NULL) {
        perror("Descriptor de archivo no especificado");
    }
    df = atoi(trozos[1]);
    if (df < 0) {
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


void memfill(char *trozos[]) {
    void *p = NULL;
    size_t nbytes = 128;  // Tamaño por defecto
    unsigned char byte = 'A';  // Byte por defecto

    // Validación de parámetros
    if (trozos[1] == NULL) {
        printf("Faltan parámetros: dirección de memoria requerida.\n");
        return;
    }

    // Convertir la dirección proporcionada a puntero
    p = (void *)strtoul(trozos[1], NULL, 16);

    // Validar el tamaño si se proporciona
    if (trozos[2] != NULL) {
        nbytes = (size_t)atoi(trozos[2]);
        if (nbytes == 0) {
            printf("Error: tamaño inválido.\n");
            return;
        }
    }

    // Leer el byte a escribir, si se proporciona
    if (trozos[3] != NULL) {
        byte = (unsigned char)trozos[3][0];
    }

    // Validar permisos y accesibilidad de la región
    if (mprotect(p, nbytes, PROT_READ | PROT_WRITE) != 0) {
        perror("Error: no se puede escribir en la dirección especificada.");
        return;
    }

    // Llenar la memoria con el byte especificado
    printf("Llenando %zu bytes en %p con '%c'...\n", nbytes, p, byte);
    LlenarMemoria(p, nbytes, byte);
}



//funcion auxiliar para memdump
void dumpMem(const char *p, int nbytes, int tam_asignado) {
    int i, j;
    unsigned char c;

    // Asegúrate de no leer más allá de la memoria asignada
    if (nbytes > tam_asignado) {
        nbytes = tam_asignado; // Limita la lectura al tamaño asignado
    }

    for(i = 0; i < nbytes; i += 25) {
        for (j = i; j < i + 25 && j < nbytes; j++) {
            c = *(p + j);
            if (c >= 32 && c <= 127){
                printf("%c ", c);
            } else {
                printf(" ");
            }
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
    char * p;
    int nbytes = 25;
    int tam_asignado = 10;  // Este valor debe coincidir con el tamaño real asignado

    if(trozos[1] == NULL)
        return;
    else {
        if(sscanf(trozos[1], "0x%p", &p) == 0 || p == NULL) {
            perror("Direccion invalida.");
            return;
        } else {
            if(trozos[2] != NULL)
                nbytes = atoi(trozos[2]);

            printf("Volcando %d bytes desde la direccion %p\n", nbytes, p);
            dumpMem(p, nbytes, tam_asignado);  // Pasa el tamaño asignado
        }
    }
}


void deallocate(char *tr[], tListM *L) {
    if (tr[1] == NULL) {
        printf("Debe especificar el tipo de memoria a liberar.\n");
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
    else if (strcmp(tr[1], "-delkey") == 0 && tr[2] != NULL) {
        char *key_str = tr[2];
        key_t clave = (key_t)strtoul(key_str, NULL, 10);  // Convierte la clave de memoria compartida
        if (clave == IPC_PRIVATE) {
            printf("Clave inválida para memoria compartida.\n");
            return;
        }

        // Llamar a la función que elimina la memoria compartida
        do_DeallocateDelkey(tr);
    }else if (strcmp(tr[1], "-shared") == 0 && tr[2] != NULL) {
        deallocateShared(tr, L);
    }else {
        // Asegurarse de que la dirección se pasa correctamente en formato hexadecimal
        void *addr = (void *)strtoull(tr[1], NULL, 16);  // Convertimos la dirección hexadecimal (ejemplo: 0x12345)

        if (addr == NULL) {
            printf("Dirección inválida para deallocar.\n");
            return;
        }

        printf("Intentando liberar la dirección: %p\n", addr);

        // Llamamos a la función para deallocar la dirección específica
        deallocate_addr(addr, L);
        return;
    }
}

void deallocateShared(char *tr[], tListM *L){
    char *key_str = tr[2];
    key_t clave = (key_t)strtoul(key_str, NULL, 10);  // Convierte la clave de memoria compartida
    if (clave == 0 || clave == IPC_PRIVATE) {
        printf("Clave inválida para memoria compartida.\n");
        return;
    }

    // Recorremos la lista de bloques de memoria para encontrar el bloque correspondiente
    int aux;
    for (aux = 0; aux <= L->lastPos; aux++) {
        if (L->itemM[aux].cmdType == SHARED && L->itemM[aux].key == clave) {
            // Intentar desasociar el bloque de memoria compartida
            if (shmdt(L->itemM[aux].dir) == -1) {
                perror("Error al desasociar el bloque de memoria compartida");
                return;
            }
            // Eliminar el bloque de la lista de memoria
            deleteItemMemList(aux, L);
            printf("Bloque de memoria compartida con clave %d liberado.\n", clave);
            return;
        }
    }

    printf("No se encontró ningún bloque de memoria compartida con clave %d.\n", clave);
}



void InsertarNodoShared(tListM *memList, void *dir, size_t tam, key_t clave) {
    // Crear un nuevo bloque de datos de tipo SHARED
    dataMem nuevoBloque;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    // Inicializar los campos del nuevo bloque
    nuevoBloque.size = tam;
    nuevoBloque.dir = dir;
    nuevoBloque.cmdType = SHARED;
    nuevoBloque.key = clave;
    strftime(nuevoBloque.date, 128, "%b %d %H:%M", tlocal);

    // Insertar el nuevo bloque en la lista
    if (!insertMemListPos(memList, nuevoBloque)) {
        printf("Error al insertar el bloque de memoria compartida.\n");
    }
}


void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListM *L) {
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
    InsertarNodoShared(L, p, s.shm_segsz, clave);
    return p;
}

void do_AllocateCreateshared (char *tr[], tListM *L) { //Crea un bloque de memoria compartida
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0]==NULL || tr[1]==NULL) {
        ImprimirListaShared(L);
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

    cl=(key_t)  strtoul(tr[2],NULL,10);

    if ((p=ObtenerMemoriaShmget(cl,0,L))!=NULL)
        printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}


void *MapearFichero(char *fichero, int protection) {
    int df, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR; // Abrir en modo lectura/escritura si es necesario

    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    if (s.st_size == 0) { // Evita mapear archivos vacíos
        fprintf(stderr, "El archivo %s está vacío.\n", fichero);
        close(df);
        return NULL;
    }

    // Usar MAP_SHARED si se permiten escrituras
    int map_flags = (protection & PROT_WRITE) ? MAP_SHARED : MAP_PRIVATE;

    p = mmap(NULL, s.st_size, protection, map_flags, df, 0);
    close(df); // Cierra el descriptor tras mapear el archivo
    if (p == MAP_FAILED) {
        perror("Error al mapear el archivo");
        return NULL;
    }

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

void do_AllocateMmap(char *arg[], tListM *L) {
    char *perm;
    void *p;
    int protection = PROT_READ; // Por defecto, permisos de lectura
    dataMem item;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    struct stat file;

    if (arg[2] == NULL) {
        ImprimirListaMmap(L);
        return;
    }

    if (stat(arg[2], &file) == -1) { // Verifica que el archivo exista
        perror("Error al obtener información del archivo");
        return;
    }

    if ((perm = arg[3]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }

    if ((p = MapearFichero(arg[2], protection)) == NULL) {
        perror("Imposible mapear fichero");
    } else {
        item.cmdType = MMAP;
        item.size = file.st_size;
        item.dir = p;
        strftime(item.date, 128, "%b %d %H:%M", tlocal);
        item.fichero.df = open(arg[2], O_RDONLY); // Guardar el descriptor del archivo
        strcpy(item.fichero.filename, arg[2]);
        if (insertMemListPos(L, item)) {
            printf("fichero %s mapeado en %p\n", arg[2], p);
        } else {
            printf("No ha sido posible mapear el archivo\n");
        }
    }
}


void do_DeallocateDelkey (char *args[]){ //función para borrar una clave de un bloque de memoria compartida
    key_t clave;
    int id;
    char *key=args[2];

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