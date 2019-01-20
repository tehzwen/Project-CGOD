#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "game.h"

// debug print map
// input map, return printed map
void printMap (map currentMap) {
    for (int i = 0; i < 81000; i++) {
        printf("%s",currentMap.object[i].objectString);
        if(i%600==1) {
            printf("\n");
        }
        //printf("Obj %d, xCoord %d, yCoord %d, Char %s\n",i ,currentMap.object[i].xCoord, currentMap.object[i].yCoord, currentMap.object[i].objectString);
    }
}

// create map
// no input, return empty map

map createMap2() {
    map newMap;

    int i = 0;
    int j = 0;

    for (i; i < 81000; i++) {

        if (i<599) {
            newMap.object[i].objectString = "X";
            newMap.object[i].xCoord = i;
            newMap.object[i].yCoord = 0;
        }

        else if (i>80401) {
            newMap.object[i].objectString = "X";
            newMap.object[i].xCoord = i - 80401;
            newMap.object[i].yCoord = 599;
        }

        else if (i%600==0) {
            newMap.object[i].objectString = "X";
            newMap.object[i].xCoord = 0;
            newMap.object[i].yCoord = i/600;
        }

        else if (i%599==0) {
            newMap.object[i].objectString = "X";
            newMap.object[i].xCoord = 0;
            newMap.object[i].yCoord = i/599;
        }

        else {
       
            int rando = rand() % 8;

            switch(rando) {
                case 1 :
                    newMap.object[i].objectString = "A";
                    newMap.object[i].xCoord = i%600;
                    newMap.object[i].yCoord = i/600;
                    break;

                case 2 : 
                    newMap.object[i].objectString = "~";
                    newMap.object[i].xCoord = i%600;
                    newMap.object[i].yCoord = i/600;
                    break;

                default : 
                    newMap.object[i].objectString = " ";
                    newMap.object[i].xCoord = i%600;
                    newMap.object[i].yCoord = i/600;
                    break;
                
                }
        }
    }

    return newMap;    
}

map createMap() {
    map newMap;

    int i = 0;
    int j = 0;

    for (i; i < 79200; i++) {

        int rando = rand() % 2;

        switch(rando) {
            case 1 :
                newMap.object[i].objectString = "A";
                newMap.object[i].xCoord = rand() % 600;
                newMap.object[i].yCoord = rand() % 400;
                break;

            default : 
                newMap.object[i].objectString = "~";
                newMap.object[i].xCoord = rand() % 600;
                newMap.object[i].yCoord = rand() % 400;
                break;
        }
    }

    for (i; i < 79800; i++) {
        newMap.object[i].objectString = "X";
        newMap.object[i].xCoord = i-79200;
        newMap.object[i].yCoord = 0;
    }

    for (i; i < 80400; i++) {
        newMap.object[i].objectString = "X";
        newMap.object[i].xCoord = i-79800;
        newMap.object[i].yCoord = 399;  
    }

    for (j; j < 400; j++) {
        newMap.object[i].objectString = "X";
        newMap.object[i].xCoord = 0;
        newMap.object[i].yCoord = j;
        i++;
    }

    j=0;

    for (j; j < 400; j++) {
        newMap.object[i].objectString = "X";
        newMap.object[i].xCoord = 599;
        newMap.object[i].yCoord = j;
        i++;
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
int main() {
    map newMap = createMap2();

    printMap(newMap);

    return 0;
}
