#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "packet.h"
#include "serverGameLogic.h"

//helper function to determine if packet already exists with that username in
//our array
bool checkIfPlayerPacketExists(Array *a, char *userName)
{
    for (int x = 0; x < a->used; x++)
    {
        printf("%s vs %s\n", a->array[x].userName, userName);

        if (strcmp(a->array[x].userName, userName) == 0)
        {
            return true;
        }
    }
    return false;
}

void initArray(Array *a, size_t initialSize)
{
    a->size = initialSize;
    a->array = (packet *)malloc(initialSize * sizeof(packet));
    a->used = 0;
}

void addToArray(Array *a, packet packVal)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (packet *)realloc(a->array, a->size * sizeof(packet));
    }
    a->array[a->used++] = packVal;
}

//need to null out the packet before returning if not found
packet getPacketFromArray(Array *a, char *userName)
{
    packet temp;

    if (checkIfPlayerPacketExists(a, userName) == true)
    {
        for (int x = 0; x < a->used; x++)
        {
            printf("%s vs %s\n", a->array[x].userName, userName);

            if (strcmp(a->array[x].userName, userName) == 0)
            {
                printf("here\n");
                temp = a->array[x];
                return temp;
            }
        }
    }
    return temp;
}

void updateClientInfo(Array *a, packet packVal)
{

    for (int x = 0; x < a->used; x++)
    {
        if (strcmp(a->array[x].userName, packVal.userName) == 0)
        {
            a->array[x].x = packVal.x;
            a->array[x].y = packVal.y;
            a->array[x].active = packVal.active;
            printf("Updated here\n");
        }
    }
}