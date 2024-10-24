//
// Created by angela on 22/10/24.
//
#include "p1.h"
#include "historial.h"
#include "p0.h"

// Struct para las opciones de stat y list
typedef struct optionBool {
    bool longOption;
    bool linkOption;
    bool accOption;
    //bool hidOption;
    //bool recaOption;
    //bool recbOption;
} optionBool;


void makefile(char *trozos[]){
	int df;

    if (trozos[1] == NULL) {
       printf("Error al crear el archivo\n");
       return;
    }

    df = open(trozos[1], O_CREAT | O_EXCL, 0664);

    if (df == -1) {
      perror("Error al abrir el archivo");
      return;
    }

    printf("Archivo %s creado exitosamente\n", trozos[1]);
    close(df);

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

// Funcion auxiliar para stat y list:
void printStatInfo(const char * name, const struct stat *st, struct optionBool opciones) {
    struct passwd *userInfo;
    struct group *groupInfo;
    char linkName[100]="", userName[20], groupName[20], time[50];

    if(opciones.longOption) {
        if(opciones.accOption)
            strftime(time, sizeof(time), "%Y/%m/%d-%H:%M", localtime(&st->st_atime));
        else
            strftime(time, sizeof(time), "%Y/%m/%d-%H:%M", localtime(&st->st_mtime));
        printf("%s", time);

        if((userInfo = getpwuid(st->st_uid)) != NULL)
            sprintf(userName, "%s", userInfo->pw_name);
        else
            sprintf(userName, "%d", st->st_uid);

        if((groupInfo = getgrgid(st->st_gid)) != NULL)
            sprintf(groupName, "%s", groupInfo->gr_name);
        else
            sprintf(groupName, "%d", st->st_gid);

        printf("%2ld (%8ld) %s %s", (long)st->st_nlink, (long)st->st_ino, userName, groupName);
        printPermissions(*st);
    }

    printf("%9ld %s", st->st_size, name);

    if(opciones.linkOption && opciones.longOption && S_ISLNK(st->st_mode)) {

        if((readlink(name, linkName, sizeof(linkName) - 1)) != -1)
            printf("->%s", linkName);
        else
            perror("Imposible acceder al link");
    }

    printf("\n");
}

void listFile(char * tr[]) {
    int i;
    struct optionBool opciones;

    opciones.longOption = false;
    opciones.linkOption = false;
    opciones.accOption = false;

    if (tr[1] == NULL) {
        cwd();
        return;
    }

    for (i = 1; tr[i] != NULL; i++) {
        if (tr[i][0] == '-') {
            // Opcion detectada
            if (tr[i][2] == 'o')
                opciones.longOption = true;
            else if (tr[i][1] == 'a')
                opciones.accOption = true;
            else if (tr[i][2] == 'i')
                opciones.linkOption = true;
            else {
                fprintf(stderr, "Opcion no valida: %s\n", tr[i]);
                return;
            }
        } else {
            struct stat st;

            if (stat(tr[i], &st) == 0)
                printStatInfo(tr[i], &st, opciones);
            else
                perror("Imposible acceder al fichero o directorio");
        }
    }
}


void listDir(char *trozos[]){
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *dirpath;
    int hid = 0,lon = 0, acc = 0, link = 0;


    if (trozos[1]==NULL)
    {
        cwd();
        return;
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
                printf(" %ld %s", fileStat.st_size, dirpath);
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

    closedir(dir);
}

void cwd(){
    char cwd[MAXIMUND];
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