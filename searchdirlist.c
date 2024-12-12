#include "searchdirlist.h"

void createEmptySearchList(tSearchList *searchList){
    searchList->lastPosSearch =0;
}

bool isEmptySearchList(tSearchList searchList){
    return (searchList.lastPosSearch == 0);
}

int firstSearchList(tSearchList searchList){
    return 0;
}

int lastSearchList(tSearchList searchList){
    return searchList.lastPosSearch;
}

int nextSearchList(tSearchList searchList, int pSearch){
    if(pSearch==searchList.lastPosSearch)
        return -1;
    else
        return ++pSearch;
}

bool insertSearchList(tSearchList *searchList, char *dir) {
    if (searchList->lastPosSearch < MAXDIRS) {
        strncpy(searchList->dirs[searchList->lastPosSearch], dir, MAXPATH - 1);
        searchList->dirs[searchList->lastPosSearch][MAXPATH - 1] = '\0';
        searchList->lastPosSearch++;
        return true;
    } else {
        return false;
    }
}

void removeSearchList(tSearchList *searchList, char *dir) {
    for (int i = 0; i < searchList->lastPosSearch; i++) {
        if (strcmp(searchList->dirs[i], dir) == 0) {
            for (int j = i; j < searchList->lastPosSearch - 1; j++) {
                strncpy(searchList->dirs[j], searchList->dirs[j + 1], MAXPATH);
            }
            searchList->lastPosSearch--;
            break;
        }
    }
}

void removePositionSearchList(tSearchList *searchList, int position) {
    if (position >= 0 && position < searchList->lastPosSearch) {
        for (int i = position; i < searchList->lastPosSearch - 1; i++) {
            strncpy(searchList->dirs[i], searchList->dirs[i + 1], MAXPATH);
        }
        searchList->lastPosSearch--;
    }
}

char *getItemSearchList(tSearchList searchList, int position) {
    if (position >= 0 && position < searchList.lastPosSearch) {
        return searchList.dirs[position];
    }
    return NULL;
}

void updateSearchList(tSearchList *searchList, char *dir) {
    for (int i = 0; i < searchList->lastPosSearch; i++) {
        if (strcmp(searchList->dirs[i], dir) == 0) {
            strncpy(searchList->dirs[i], dir, MAXPATH);
            break;
        }
    }
}

tSearchList LibroDeBusqueda;

// Función para encontrar el directorio de un ejecutable
char *Ejecutable(char *s) {
    static char path[MAXPATH];
    struct stat st;

    if (s == NULL)
        return NULL;

    // Verificar si es una ruta absoluta o relativa
    if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
        return s;

    // Buscar en la lista de búsqueda global
    for (int i = firstSearchList(LibroDeBusqueda); i < lastSearchList(LibroDeBusqueda); i = nextSearchList(LibroDeBusqueda, i)) {
        char *dir = getItemSearchList(LibroDeBusqueda, i);
        if (dir == NULL)
            continue;

        snprintf(path, MAXPATH, "%s/%s", dir, s);
        if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR)) {
            return path;
        }
    }

    return NULL;
}