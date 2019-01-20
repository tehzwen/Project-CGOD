#ifndef MAPBUFFER_H
#define MAPBUFFER_H
#include "gameObjectArray.h"

int getArraySize(char *buffer);

void incrementBufferPacketArraySize(char *buffer);

gameObjectArray getArrayFromBuffer(char *buffer, int desiredIndex);

void addArrayToBuffer(char *buffer, gameObjectArray packVal, int *currentIndex);

#endif