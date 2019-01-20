#ifndef GAMEOBJECTARRAY_H
#define GAMEOBJECTARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>
#include <netinet/in.h>

typedef struct
{
    int yCoord;
    int xCoord;
    int id;
    char objectString[10];


} gameObject;

typedef struct{
    int size;
    int used;
    gameObject *array;
    int currID;
} gameObjectArray;


void initObjectArray(gameObjectArray *a, size_t initialSize);

void addToObjArray(gameObjectArray *a, gameObject objVal);

gameObject getGameObjectFromArray(gameObjectArray *a, int id);

#endif