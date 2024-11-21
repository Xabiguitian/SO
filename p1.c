
#include "p1.h"
#include "historial.h"
#include "p0.h"



void makefile(char *trozos[]){
	int id;

    if (trozos[1] == NULL) {
       printf("Error al crear el archivo\n");
       return;
    }

    id = open(trozos[1], O_CREAT | O_EXCL, 0664);

    if (id == -1) {
      perror("Error al abrir el archivo");
      return;
    }

    printf("Archivo %s creado exitosamente\n", trozos[1]);
    close(id);

}

void makedir(char *trozos[]){
    if (trozos[1] == NULL) {
        printf("Error al crear el directorio %s\n", trozos[1]);
        return;
    }

    if (mkdir(trozos[1], 0775) == -1) {
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
void formatDate(char *buffer, size_t size, time_t modTime) {
    struct tm *timeinfo = localtime(&modTime);
    strftime(buffer, size, "%Y/%m/%d-%H:%M", timeinfo);
}
void listFile(char *trozos[]) {
    char *dirpath = NULL;
    int lon = 0, acc = 0, link = 0;

    if (trozos[1] == NULL) {
        cwd();
        return;
    }

    for (int i = 1; trozos[i] != NULL; i++) {
        if (strcmp(trozos[i], "-long") == 0) {
            lon = 1;
        } else if (strcmp(trozos[i], "-acc") == 0) {
            acc = 1;
        } else if (strcmp(trozos[i], "-link") == 0) {
            link = 1;
        } else {
            dirpath = trozos[i];
        }
    }

    if (dirpath == NULL) {
        fprintf(stderr, "No se especificó ningún archivo o directorio.\n");
        return;
    }

    struct stat st;
    if (stat(dirpath, &st) == 0) {
        if (lon || acc || link) {
            if (lon) {
                char date[20];
                formatDate(date, sizeof(date), st.st_mtime);
                struct passwd *pw = getpwuid(st.st_uid);
                struct group *gr = getgrgid(st.st_gid);
                printf("%s   %ld (%ld)     %s     %s ", date, (long)st.st_nlink, (long)st.st_ino, pw ? pw->pw_name : "(null)", gr ? gr->gr_name : "(null)");
                printPermissions(st);
                printf("     %lld %s\n", (long long)st.st_size, dirpath);
            }

            if (acc) {
                printf("%s", ctime(&st.st_atime));
            }

            if (link && S_ISLNK(st.st_mode)) {
                char linkDest[MAX];
                ssize_t len = readlink(dirpath, linkDest, sizeof(linkDest) - 1);
                if (len != -1) {
                    linkDest[len] = '\0';
                    printf("%s -> %s\n", dirpath, linkDest);
                } else {
                    printf("%s (enlace simbólico, pero no se pudo obtener el destino)\n", dirpath);
                }
            } else if (link) {
                printf("%lld %s\n", (long long)st.st_size, dirpath);
            }
        }else {
            printf("%lld %s\n", (long long)st.st_size, dirpath);
        }
    } else {
        perror("Imposible acceder al fichero o directorio");
    }
}


void listDir(char *trozos[]){
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *dirpath = ".";
    int hid = 0,lon = 0, acc = 0, link = 0;


    if (trozos[1]==NULL)
    {
        cwd();
        return;
    } else{
        int i;
        for(i=1;trozos[i] != NULL;i++){
            if (strcmp(trozos[i], "-hid") == 0 || strcmp(trozos[i], "-long") == 0 ||strcmp(trozos[i], "-acc") == 0 ||strcmp(trozos[i], "-link") == 0)
            {
            if (strcmp(trozos[i], "-hid") == 0) hid =1;
            if (strcmp(trozos[i], "-long") == 0) lon =1;
            if (strcmp(trozos[i], "-acc") == 0) acc =1;
            if (strcmp(trozos[i], "-link") == 0) link =1;
            }else {
                dirpath = trozos[i];
            }
        }
    }

    if ((dir = opendir(dirpath)) == NULL){
        perror("Error al abrir el directorio");
        return;
    }

    printf("**** %s:\n", dirpath);
    while ((entry = readdir(dir)) != NULL) {
        if(!hid && entry->d_name[0] == '.'){
            continue;
        }

        char filepath[MAX];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);

        if(stat(filepath, &fileStat) == -1){
            perror("Error al obtener información del archivo");
            continue;
        }

        if (lon || acc || link)
        {
            if (lon)
            {
            char date[20];
            formatDate(date, sizeof(date), fileStat.st_mtime);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            struct group *gr = getgrgid(fileStat.st_gid);
            printf("%s   %ld (%ld)     %s     %s ", date, (long)fileStat.st_nlink, (long)fileStat.st_ino, pw->pw_name, gr->gr_name);
            printPermissions(fileStat);
            printf("     %lld %s\n", (long long)fileStat.st_size, entry->d_name);
            }

            if (acc && !lon)
            {
                printf("%s -> %s", entry->d_name,ctime(&fileStat.st_atime));
            }

            if (link && S_ISLNK(fileStat.st_mode) && !lon)
            {
                char link[MAX];
                ssize_t len = readlink(filepath, link, sizeof(link) - 1);
                if (len != -1)
                {
                    link[len] = '\0';
                    printf("%s -> %s\n", entry->d_name, link);
                } else {
                    printf("%s (enlace simbólico, pero no se pudo obtener el destino)\n", entry->d_name);
                }

            }
        }else {
            printf("%lld %s\n", (long long)fileStat.st_size, entry->d_name);
        }

    }

    closedir(dir);
}
void cwd(){
    char cwd[MAXIMUND];
    getcwd(cwd,sizeof cwd);
    printf("%s\n",cwd);
}

void reclist(char *trozos[]) {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *direccion = ".";
    int hid = 0, lon = 0, acc = 0, link = 0;


    if (trozos[1] == NULL) {
        cwd();
        return;
    } else {
        direccion = trozos[1];
        for (int i = 1; trozos[i] != NULL; i++) {
            if (strcmp(trozos[i], "-hid") == 0) hid = 1;
            else if (strcmp(trozos[i], "-long") == 0) lon = 1;
            else if (strcmp(trozos[i], "-acc") == 0) acc = 1;
            else if (strcmp(trozos[i], "-link") == 0) link = 1;
            else direccion = trozos[i];
        }
    }


    if ((dir = opendir(direccion)) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("\n*****%s\n", direccion);


    struct dirent *archivos[1024];
    struct dirent *carpetas[1024];
    int numArchivos = 0, numCarpetas = 0;


    while ((entry = readdir(dir)) != NULL) {

        if (!hid && entry->d_name[0] == '.') continue;
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (lstat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }


        if (S_ISDIR(fileStat.st_mode)) {
            carpetas[numCarpetas++] = entry;
        } else {
            archivos[numArchivos++] = entry;
        }
    }


    for (int i = 0; i < numArchivos; i++) {
        entry = archivos[i];
        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (lstat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (lon) {
            char date[20];
            formatDate(date, sizeof(date), fileStat.st_mtime);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            struct group *gr = getgrgid(fileStat.st_gid);
            printf("%s   %ld (%ld)     %s     %s ", date, (long)fileStat.st_nlink, (long)fileStat.st_ino, pw->pw_name, gr->gr_name);
            printPermissions(fileStat);
            printf("     %lld %s\n", (long long)fileStat.st_size, entry->d_name);
        } else {
            printf("%lld %s\n", (long long)fileStat.st_size, entry->d_name);
        }

        if (acc) printf(" Último acceso: %s", ctime(&fileStat.st_atime));
    }


    for (int i = 0; i < numCarpetas; i++) {
        entry = carpetas[i];
        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (lstat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }


        if (lon) {
            char date[20];
            formatDate(date, sizeof(date), fileStat.st_mtime);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            struct group *gr = getgrgid(fileStat.st_gid);
            printf("%s   %ld (%ld)     %s     %s ", date, (long)fileStat.st_nlink, (long)fileStat.st_ino, pw->pw_name, gr->gr_name);
            printPermissions(fileStat);
            printf("     %lld %s\n", (long long)fileStat.st_size, entry->d_name);
        } else {
            printf("%lld %s\n", (long long)fileStat.st_size, entry->d_name);
        }

        if (acc) printf(" Último acceso: %s", ctime(&fileStat.st_atime));


        char *newTrozos[6];
        int argIndex = 0;

        if (lon) newTrozos[argIndex++] = "-long";
        if (acc) newTrozos[argIndex++] = "-acc";
        if (link) newTrozos[argIndex++] = "-link";
        if (hid) newTrozos[argIndex++] = "-hid";
        newTrozos[argIndex++] = direccionArchivo;
        newTrozos[argIndex] = NULL;

        reclist(newTrozos);
    }
    closedir(dir);
}

void revlist(char *trozos[]) {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *direccion = ".";
    int hid = 0, lon = 0, acc = 0, link = 0;

    if (trozos[1] == NULL) {
        cwd();
        return;
    } else {
        direccion = trozos[1];
        for (int i = 1; trozos[i] != NULL; i++) {
            if (strcmp(trozos[i], "-hid") == 0) hid = 1;
            else if (strcmp(trozos[i], "-long") == 0) lon = 1;
            else if (strcmp(trozos[i], "-acc") == 0) acc = 1;
            else if (strcmp(trozos[i], "-link") == 0) link = 1;
            else direccion = trozos[i];
        }
    }

    if ((dir = opendir(direccion)) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("\n*****%s\n", direccion);

    struct dirent *archivos[1024];
    struct dirent *carpetas[1024];
    int numArchivos = 0, numCarpetas = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!hid && entry->d_name[0] == '.') continue;
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (lstat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)) {
            carpetas[numCarpetas++] = entry;
        } else {
            archivos[numArchivos++] = entry;
        }
    }

    for (int i = 0; i < numCarpetas; i++) {
        entry = carpetas[i];
        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (lstat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (lon) {
            char date[20];
            formatDate(date, sizeof(date), fileStat.st_mtime);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            struct group *gr = getgrgid(fileStat.st_gid);
            printf("%s   %ld (%ld)     %s     %s ", date, (long)fileStat.st_nlink, (long)fileStat.st_ino, pw->pw_name, gr->gr_name);
            printPermissions(fileStat);
            printf("     %lld %s\n", (long long)fileStat.st_size, entry->d_name);
        } else {
            printf("%lld %s\n", (long long)fileStat.st_size, entry->d_name);
        }

        if (acc) printf(" Último acceso: %s", ctime(&fileStat.st_atime));

        char *newTrozos[6];
        int argIndex = 0;

        if (lon) newTrozos[argIndex++] = "-long";
        if (acc) newTrozos[argIndex++] = "-acc";
        if (link) newTrozos[argIndex++] = "-link";
        if (hid) newTrozos[argIndex++] = "-hid";
        newTrozos[argIndex++] = direccionArchivo;
        newTrozos[argIndex] = NULL;

        reclist(newTrozos);
    }

    for (int i = 0; i < numArchivos; i++) {
        entry = archivos[i];
        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (lstat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (lon) {
            char date[20];
            formatDate(date, sizeof(date), fileStat.st_mtime);
            struct passwd *pw = getpwuid(fileStat.st_uid);
            struct group *gr = getgrgid(fileStat.st_gid);
            printf("%s   %ld (%ld)     %s     %s ", date, (long)fileStat.st_nlink, (long)fileStat.st_ino, pw->pw_name, gr->gr_name);
            printPermissions(fileStat);
            printf("     %lld %s\n", (long long)fileStat.st_size, entry->d_name);
        } else {
            printf("%lld %s\n", (long long)fileStat.st_size, entry->d_name);
        }

        if (acc) printf(" Último acceso: %s", ctime(&fileStat.st_atime));
    }
    closedir(dir);
}


void erase(char *trozos[]){
    struct stat fileStat;
    char *target;

    if (trozos[1] == NULL)
    {
        printf("Debe especificar un archivo o directorio para eliminar\n");
        return;
    }

    target = trozos[1];

    if (stat(target, &fileStat) == -1)
    {
        perror("Error al obtener información del archivo o directorio");
        return;
    }

    if (S_ISREG(fileStat.st_mode)){
        if (remove(target) == 0)
        {
            printf("Archivo eliminado: %s\n", target);
        }else{
            perror("Error al eliminar el archivo");
        }

    } else if (S_ISDIR(fileStat.st_mode)){
        DIR *dir = opendir(target);
        if (dir == NULL)
        {
            perror("Error al abrir el directorio");
            return;
        }

        struct dirent *entry;
        int isEmpty = 1;

        while((entry = readdir(dir)) != NULL){
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
                isEmpty=0;
                break;
            }
        }
        closedir(dir);

        if (isEmpty)
        {
            if (remove(target) == 0)
            {
                printf("Directorio vacío eliminado: %s\n", target);
            } else {
                perror("Error al eliminar el directorio");
            }
        }else{
            printf("Error: El directorio no está vacío y no se puede eliminar\n");
        }

    }else{
        printf("Error: El objetivo no es un archivo ni un directorio\n");
    }

}

void delrec(char *path){
    struct stat fileStat;

    if (stat(path, &fileStat) == -1)
    {
        perror("Error al obtener información del archivo o directorio");
        return;
    }

    if (S_ISREG(fileStat.st_mode))
    {
        if (remove(path) == 0)
        {
            printf("Archivo eliminado: %s\n", path);
        }else{
            perror("Error al eliminar el archivo");
        }
    }else if(S_ISDIR(fileStat.st_mode)){
        DIR *dir = opendir(path);
        if(dir == NULL){
            perror("Error al abrir el directorio");
            return;
        }

        struct dirent *entry;
        char filepath[MAX];

        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);

            delrec(filepath);
        }

        closedir(dir);

        if(rmdir(path) == 0){
            printf("Directorio eliminado: %s\n", path);
        }else {
            perror("Error al eliminar el directorio");
        }
    } else {
        printf("Error: El objetivo no es un archivo ni un directorio\n");
    }
}