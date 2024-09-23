//
// Created by angela on 23/09/24.
//

#ifndef STATICLIST_H
#define STATICLIST_H


#include "types.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define NULL -1
#define MAX 25

typedef int tPos;

typedef tSong tItem;

typedef struct {
    tItem data[MAX];
    tPosS lastPos;
}tList;



void createEmptyList(tList *L);
void deleteAtPosition(tPos p, tList *L);
bool isEmptyList(tList L);
tPos first(tList L);
tPos next(tPos p, tList L);
tPos last(tList l);
tPos previous(tPos p,tList L);
tItem getItem(tPos p, tList L);
void updateItem(tItem d, tPos p, tList *L);
tPos findItem(tSongTitle d,tList L);



#endif //STATICLIST_H
