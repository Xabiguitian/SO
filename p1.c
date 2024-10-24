//
// Created by angela on 22/10/24.
//
#include "p1.h"

void makefile(char *trozos[]){

    if (trozos[1] == NULL) {
       printf("Error al crear el archivo %s\n", trozos[1]);
    }

    FILE *file = fopen(trozos[1], "w");

    if (file == NULL) {
      perror("Error al abrir el archivo");
      return;
    }

    printf("Archivo %s creado exitosamente\n", trozos[1]);
    fclose(file);

}

void makedir(char *trozos[]){
    if (trozos[1] == NULL) {
        printf("Error al crear el directorio %s\n", trozos[1]);
    }

    if (mkdir(trozos[1], 0700) == -1) {
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

void listFile(char *trozos[]){
    struct stat fileStat;
    char *nombre, cwd[MAXIMUN];
    int lon = 0, acc = 0, link = 0;

    if (trozos[1] == NULL){
        getcwd(cwd,sizeof cwd);
        nombre = cwd;
    } else {
        for(int i=1;trozos[i] != 0;i++){
        if (strcmp(trozos[i], "-long") == 0) lon =1;
        else if (strcmp(trozos[i], "-acc") == 0) acc =1;
        else if (strcmp(trozos[i], "-link") == 0) link =1;
        else nombre = trozos[i];
    }
    }

    if (stat(nombre, &fileStat) == -1){
        perror("Error al obtener información del archivo o directorio");
        return;
    }
    

    if (lon){
        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group *gr = getgrgid(fileStat.st_gid);

        if (pw == NULL || gr == NULL) {
            perror("Error al obtener información de usuario o grupo");
            return;
        }

		printf("%s %s %s ", ctime(&fileStat.st_mtime), getpwuid(fileStat.st_uid)->pw_name, getgrgid(fileStat.st_gid)->gr_name);
        printPermissions(fileStat);
        printf(" %lld %s", fileStat.st_size, nombre);
    }

    if (acc){
        printf("Último acceso: %s\n", ctime(&fileStat.st_atime));
    }

    if (link && S_ISLNK(fileStat.st_mode)) {
        char linkTarget[MAXIMUN];
        ssize_t len = readlink(nombre, linkTarget, sizeof(linkTarget) - 1);

        if (len != -1) {
            linkTarget[len] = '\0';
            printf("%s -> %s\n", nombre, linkTarget);
        } else {
            perror("Error al leer el enlace simbólico");
        }
    }
    printf("%lld %s\n", fileStat.st_size, nombre);
}

void listDir(char *trozos[]){
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *dirpath;
    int hid = 0,lon = 0, acc = 0, link = 0;

    if (trozos[1]==NULL)
    {
        dirpath = ".";
    } else{
        for(int i=1;trozos[i] != NULL;i++){
            if (strcmp(trozos[i], "-hid") == 0) hid =1;
            else if (strcmp(trozos[i], "-long") == 0) lon =1;
            else if (strcmp(trozos[i], "-acc") == 0) acc =1;
            else if (strcmp(trozos[i], "-link") == 0) link =1;
            else dirpath = trozos[i];
        }
    }
    

    if ((dir = opendir(dirpath)) == NULL){
        perror("Error al abrir el directorio");
        return;
    }

    printf("Archivos y directorios en %s:\n", dirpath);
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

        if (S_ISDIR(fileStat.st_mode) || S_ISLNK(fileStat.st_mode)){
            if (lon)
            {
                struct passwd *pw = getpwuid(fileStat.st_uid);
                struct group *gr = getgrgid(fileStat.st_gid);

                if (pw == NULL || gr == NULL) {
                    perror("Error al obtener información de usuario/grupo");
                    continue;
                }
                printf("%s %ld (%ld) %s %s ", ctime(&fileStat.st_atime), (long)fileStat.st_nlink, (long)fileStat.st_ino, pw->pw_name, gr->gr_name);
                printPermissions(fileStat);
                printf(" %lld %s", fileStat.st_size, dirpath);
            }

            if (acc)
            {
                printf("%s", ctime(&fileStat.st_atime));
            }

            if (link && S_ISLNK(fileStat.st_mode))
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
                
            } else {
                printf("%s\n", entry->d_name);
            }
            
            
        }

    }

    closedir(dir);
}

void cwd(){
    char cwd[MAXIMUN];
    getcwd(cwd,sizeof cwd);
    printf("%s\n",cwd);
}

void reclist (char *trozos[]){
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *direccion;

    if (trozos[1] == NULL) {
        direccion = ".";
    } else {
        direccion = trozos[1];
    }

    if ((dir = opendir(direccion)) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("Contenido de: %s\n", direccion);
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] == '.'){
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (stat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)) {
            printf("%s\n", direccionArchivo);
            char *trozos2[] = {NULL, direccionArchivo};
            reclist(trozos2);
        }
    }

    closedir(dir);
}

void revlist (char *trozos[]){
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *direccion;

    if (trozos[1] == NULL) {
        direccion = ".";
    } else {
        direccion = trozos[1];
    }

    if ((dir = opendir(direccion)) == NULL) {
        perror("Error al abrir el directorio");
        return;
    }

    printf("Contenido de: %s\n", direccion);
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] == '.'){
            continue;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char direccionArchivo[MAX];
        snprintf(direccionArchivo, sizeof(direccionArchivo), "%s/%s", direccion, entry->d_name);

        if (stat(direccionArchivo, &fileStat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)) {
            //Todo igual que en reclist pero el printf se hace después
            char *trozos2[] = {NULL, direccionArchivo};
            reclist(trozos2);
            printf("%s\n", direccionArchivo);
        }
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