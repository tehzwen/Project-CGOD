#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "gameObjectArray.h"
#include "mapBuffer.h"

int getObjectArraySize(char * buffer){
    short int tempSize;
    memcpy(&tempSize, &(buffer[0]), sizeof(tempSize));
    return tempSize;
}

void incrementMapBufferArraySize(char * buffer){
    short int tempSize;
    memcpy(&tempSize, &(buffer[0]), sizeof(tempSize));
    tempSize++;
    memcpy(&(buffer[0]), &tempSize, sizeof(tempSize));

}

//gets 3rd and 4th bytes that will represent if the packet is a map packet
int checkIfMapData(char * buffer){
    short int tempSize;
    memcpy(&tempSize, &(buffer[2]), sizeof(tempSize));
    return tempSize;
}

gameObject getObjectFromBuffer(char *buffer, int desiredIndex)
{
    gameObject temp;

    //desiredIndex * sizeof(packet) essentially indexes through the buffer for each
    //packet inside the buffer
    memcpy(&temp, &(buffer[4 + (desiredIndex * sizeof(gameObject))]), sizeof(gameObject));

    return temp;
}

void addObjectToBuffer(char *buffer, gameObject gameObjVal, int * currentIndex)
{
    memcpy(&(buffer[4 + (*currentIndex * sizeof(gameObject))]), &gameObjVal, sizeof(gameObject));
    incrementMapBufferArraySize(buffer);
    (*currentIndex)++;
}


/*
int main()
{

    
    gameObject temp = {1, 2, 1, "X"};
    gameObject temp2 = {3, 4, 2, "Y"};

    short int size = 0;
    int index = 0;

    char buffer[sizeof(size) + (sizeof(gameObject) * 2)];

    buffer[0] = 1;
    buffer[1] = size;
    addObjectToBuffer(buffer, temp, &index);
    addObjectToBuffer(buffer, temp2, &index);

    int x = getObjectArraySize(buffer);

    printf("here: %d\n", x);

    gameObject tempObj = getObjectFromBuffer(buffer, 1);
    printf("%s\n", tempObj.objectString);

} */