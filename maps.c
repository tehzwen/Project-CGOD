#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "game.h"


int xRes = 600;
int yRes = 400;


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

void initObjectArray(gameObjectArray *a, size_t initialSize)
{
    a->size = initialSize;
    a->array = (gameObject *)malloc(initialSize * sizeof(gameObject));
    a->used = 0;
    a->currID = 0;
}

// debug print map
// input map, return printed map
void printMap(gameObjectArray currentMap)
{
    for (int i = 0; i < (xRes * yRes)/3; i++)
    {
        printf("%s", currentMap.array[i].objectString);
        if (i % 600 == 1)
        {
            printf("\n");
        }
        //printf("Obj %d, xCoord %d, yCoord %d, Char %s\n",i ,currentMap.object[i].xCoord, currentMap.object[i].yCoord, currentMap.object[i].objectString);
    }
}

// create map
// no input, return empty map

gameObjectArray createMap2()
{
    gameObjectArray newMap;
    gameObject temp;

    initObjectArray(&newMap, 2);

    int i = 0;
    int j = 0;

    for (i; i < 81000; i++)
    {

        if (i < 599)
        {
            temp.id = newMap.currID;
            temp.objectString = "X";
            temp.xCoord = i;
            temp.yCoord = 0;
            addToObjArray(&newMap, temp);
        }

        else if (i > 80401)
        {

            temp.id = newMap.currID;
            temp.objectString = "X";
            temp.xCoord = i - 80401;
            temp.yCoord = 599;
            addToObjArray(&newMap, temp);
        }

        else if (i % 600 == 0)
        {

            temp.id = newMap.currID;
            temp.objectString = "X";
            temp.xCoord = 0;
            temp.yCoord = i / 600;
            addToObjArray(&newMap, temp);
        }

        else if (i % 599 == 0)
        {

            temp.id = newMap.currID;
            temp.objectString = "X";
            temp.xCoord = 0;
            temp.yCoord = i / 599;
            addToObjArray(&newMap, temp);
        }

        else
        {

            int rando = rand() % 100;

            switch (rando)
            {
            case 1:

                temp.id = newMap.currID;
                temp.objectString = "A";
                temp.xCoord = i % 600;
                temp.yCoord = i / 600;
                addToObjArray(&newMap, temp);
                break;

            case 2:

                temp.id = newMap.currID;
                temp.objectString = "~";
                temp.xCoord = i % 600;
                temp.yCoord = i / 600;
                addToObjArray(&newMap, temp);

                break;

            default:

                temp.id = newMap.currID;
                temp.objectString = " ";
                temp.xCoord = i % 600;
                temp.yCoord = i /600;
                addToObjArray(&newMap, temp);
                break;
            }
        }
    }

    return newMap;
}
// random character gen
// input none, return new char
/*
char createTile() { //error
    tile current;
    int rando = rand() % 3;
    switch(rando) {
        case 1 :
            current.objectString = "A";
            current.xCoord = rand() % 50;
            current.yCoord = rand() % 50;
            break;
        case 2 : 
            current.objectString = "~";
            current.xCoord = rand() % 50;
            current.yCoord = rand() % 50;
            break;
        default :
            break;
    }
    return current.objectString, current.xCoord, current.yCoord;
}
*/
// populate map
// input map, return map

//main
int main()
{
    gameObjectArray newMap = createMap2();

    printMap(newMap);

    return 0;
}