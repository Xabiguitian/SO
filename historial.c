/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 1
 * AUTHOR 1: Xabier Guitián López LOGIN 1: x.guitian@udc.es
 * AUTHOR 2: Lucas Marqués Núñez LOGIN 2: lucas.marques@udc.es
 * GROUP: 2.4
 * DATE: 08 / 03 / 24
 */
#include "historial.h"
#include <stdbool.h>
#include <string.h>

/*
Objetivo: Crea una lista vacía.
 Entrada: puntero lista
 Salida: tList list vacía
 Postcondiciones: La lista queda inicializada y no contiene elementos.
 */
void createEmptyList(historic *h) {
    h->lastPos = HNULL;
}

/*
Objetivo: Determina si la lista está vacía.
 Entrada: list a evaluar
 Salida: true si está vacía, false si no lo está
 Precondición: La lista debe estar previamente inicializada.
 */
bool isEmptyList(historic h) {
    return h.lastPos == HNULL;
}

/*
 Objetivo: Inserta un elemento en la lista de forma ordenada en base al campo userName.
 Entrada: item a insertar y puntero a lista
 Salida: Devuelve un valor true si el elemento fue insertado; false en caso contrario.
 Precondición: La lista debe estar previamente inicializada.
 Postcondición: Las posiciones de los elementos de la lista posteriores a la del elemento insertado pueden haber variado.
 */
bool insertItem(char *command, historic *h) {
    if (h->lastPos >= MAX) {
        return false; // List full
    }

    if(h->lastPos==HNULL){
        h->command[h->lastPos++] = command;
        h->lastPos = 0;
    }else {
        h->command[h->lastPos++] = command;
        h->lastPos++;
    }
    return true;
}

/*
 Objetivo: Devuelve el contenido del elemento que ocupa la posición indicada.
 Entrada: posición del elemento y lista
 Salida: item de la posición indicada
 Precondición: La posición indicada es una posición válida en la lista.
 Precondición: La lista debe estar previamente inicializada.
 */
char getItem(int pos, historic h) {

    return *h.command[pos];
}