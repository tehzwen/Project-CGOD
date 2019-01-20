#ifndef SERVERGAMELOGIC_H
#define SERVERGAMELOGIC_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    packet *array;
    size_t used;
    size_t size;

} Array;

typedef struct{
    int y;
    int x;

} tuple;

void updateClientInfo(Array *a, packet packVal);

bool checkIfPlayerPacketExists(Array *a, char *userName);

packet getPacketFromArray(Array *a, char *userName);

void addToArray(Array *a, packet packVal);

void initArray(Array *a, size_t initialSize);


#endif