
//
// Created by angela on 3/12/24.
//

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

bool insertSearchList(tSearchList *searchList, char dir){
  if(searchList->lastPosSearch <MAXDIRS||MAXPATH)
}

void removeSearchList(tSearchList *searchList, char dir){

}
void removePositionSearchList(tSearchList *searchList, int position){

}
tSearchList getItemSearchList(char dir){

}
void updateSearchList(tSearchList *searchList, char dir){

}

