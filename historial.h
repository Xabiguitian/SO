/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 1
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#include <stdbool.h>

#define MAX 4096
#define HNULL -1

typedef struct {
    char *command[MAX];
    int lastPos;
} historic;


void createEmptyList(historic *h);
bool isEmptyList(historic h);
bool insertItem(char *command, historic *h);
char getItem(int pos, historic h);