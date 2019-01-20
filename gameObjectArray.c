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
#include "gameObjectArray.h"

void initObjectArray(gameObjectArray *a, size_t initialSize)
{
    a->size = initialSize;
    a->array = (gameObject *)malloc(initialSize * sizeof(gameObject));
    a->used = 0;
    a->currID = 0;
}

void addToObjArray(gameObjectArray *a, gameObject objVal)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (gameObject *)realloc(a->array, a->size * sizeof(gameObject));
    }
    a->array[a->used++] = objVal;
    a->currID++;
}

gameObject getGameObjectFromArray(gameObjectArray *a, int id)
{
    gameObject temp;

    for (int x = 0; x < a->used; x++)
    {
        printf("here %d\n", a->array[x].id);

        if (a->array[x].id == id)
        {
            temp = a->array[x];
            return temp;
        }
    }

    return temp;
} 