//
// Created by angela on 16/11/24.
//
#include "memlist.h"

void createEmptyMemList(tListM* memList) {
  memList->lastPos=-1;

}

bool isEmptyMemList(tListM memList) {
  return memList.lastPos==-1;
}

int nextMemListPos(int pMem, tListM memList) {
  if(pMem==memList.lastPos)
    return -1;
  else
    return ++pMem;
}

int prevMemListPos(int pMem, tListM memList) {
  return --pMem;
}

int firstMemListPos( tListM memList) {
  return 0;
}

int lastMemListPos(tListM memList) {
  return memList.lastPos;
}


bool insertMemListPos(tListM* memList, dataMem m) {
  if(memList->lastPos<MAX_LISTMEM){
    memList->lastPos++;
    memList->itemM[memList->lastPos]=m;
    return true;
  }else
        return false;

}


dataMem getDataItemList(tListM memList, int pMem) {
  return memList.itemM[pMem];

}


void freeMemList(tListM *memList) {
  int pos;
  dataMem item;

  for(pos=firstMemListPos(*memList);pos<=lastMemListPos(*memList);pos++){
    item=getDataItemList(*memList,pos);

    if(item.cmdType==MALLOC)
      free(item.dir);
    else if(item.cmdType==SHARED){
      if(shmget(item.Union.key,0,0)!=-1){
        if(shmdt(item.dir)==-1)
          return;
        }
    }else if(item.cmdType==MMAP)
        munmap(item.dir, item.size);
  }

}

void deleteItemMemList( int pMem,tListM* memList) {
  int i;
  for(i=0; i<lastMemListPos(*memList);i++){
    if(i>=pMem)
      memList->itemM[i]=memList->itemM[i+1];
  }
  memList->lastPos--;
}

void deleteMemList(tListM* memList, int pMem) {
  int i;
  for(i=0;i<=memList->lastPos;i--){
    deleteItemMemList(i,memList);
    memList->lastPos--;
  }
}
