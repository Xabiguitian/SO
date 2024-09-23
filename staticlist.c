//
// Created by angela on 23/09/24.
//

#include <staticlist.h>


void createEmptyList(tList *L){
  L->lastPos=NULL;

}

bool insertItem(tItem d, tPos p, tList *L) {
    tPos i;
    if (L->lastPos == MAX - 1)
        return false;
    else {
        L->lastPos++;
        if (p == NULL) {
            L->data[L->lastPos] = d;
        } else {
            for (i = L->lastPos; i >= p + 1; i--) {
                L->data[i] = L->data[i - 1];
            }
            L->data[i] = d;
        }
        return true;
    }
}

void deleteAtPosition(tPos p, tList *L){
    tPos i;
    L->lastPos--;

    for(i=p;i<=L->lastPos;i++)
    {
        L->data[i]=L->data[i + 1];

}
bool isEmptyList(tList L){
        return (L.lastPos==NULL);
    }


tPos first(tList L){
    return 0;

}

tPos next(tPos p, tList L){
    if (p==L.lastPos)
        return NULL;
    else
        return ++p;

}
tPos last(tList L){
    return L.lastPos;
}
tPos previous(tPos p,tList L){
    return --p;
}
tItem getItem(tPos p, tList L){
    return L.data[p];
}
void updateItem(tItem d, tPos p, tList *L){
    L->data[p]=d;
}

tPos findItem(tSongTitle d,tList L){
    tPos p;
    if(L.lastPos==NULL)
        return NULL;
    else
    {
        for(p= 0;(p<=L.lastPos) && strcmp(L.data[p].songTitle, d) != 0; p++);  .
        if(strcmp(L.data[p].songTitle, d) == 0)
            return p;
        else
            return NULL;
    }
}