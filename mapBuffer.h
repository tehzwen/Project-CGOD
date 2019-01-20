#ifndef MAPBUFFER_H
#define MAPBUFFER_H
#include "gameObjectArray.h"

gameObject getObjectFromBuffer(char *buffer, int desiredIndex);

int getObjectArraySize(char * buffer);

void addObjectToBuffer(char *buffer, gameObject gameObjVal, int * currentIndex);

void incrementMapBufferArraySize(char * buffer);

int checkIfMapData(char * buffer);

#endif