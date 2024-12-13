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
    if (searchList->lastPosSearch >= MAXDIRS) {
        return false; // Lista llena
    }

    strncpy(searchList->dirs[searchList->lastPosSearch], dir, MAXPATH - 1);
    searchList->dirs[searchList->lastPosSearch][MAXPATH - 1] = '\0';
    searchList->lastPosSearch++;

    return true;
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
    static char item[MAXPATH]; // Buffer estático
    if (position < 0 || position >= searchList.lastPosSearch) {
        return NULL; // Índice inválido
    }
    strncpy(item, searchList.dirs[position], MAXPATH - 1);
    item[MAXPATH - 1] = '\0'; // Asegurar terminación nula
    return item;
}


void updateSearchList(tSearchList *searchList, char *dir) {
    for (int i = 0; i < searchList->lastPosSearch; i++) {
        if (strcmp(searchList->dirs[i], dir) == 0) {
            strncpy(searchList->dirs[i], dir, MAXPATH);
            break;
        }
    }
}

char *Ejecutable(char *s, tSearchList LibroDeBusqueda) {
    static char path[NAMEMAX];
    struct stat st;

    if (s == NULL){
        return NULL;
    }

    if (s[0] == '/' || !strncmp(s, "./", 2) || !strncmp(s, "../", 3))
        return s;
        
    for (int i = firstSearchList(LibroDeBusqueda); 
        i < lastSearchList(LibroDeBusqueda); 
        i = nextSearchList(LibroDeBusqueda, i)) {
        char *dir = getItemSearchList(LibroDeBusqueda, i);

        if (dir == NULL){
            continue;
        }

        snprintf(path, NAMEMAX, "%s/%s", dir, s);

        if (lstat(path, &st) != -1 && (st.st_mode & S_IXUSR))
            return path;
    }

    return NULL;
}