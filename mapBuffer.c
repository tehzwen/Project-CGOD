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

int getArraySize(char *buffer)
{
    short int tempSize;
    memcpy(&tempSize, &(buffer[0]), sizeof(tempSize));
    return tempSize;
}

void incrementBufferPacketArraySize(char *buffer)
{
    short int tempSize;
    memcpy(&tempSize, &(buffer[0]), sizeof(tempSize));
    tempSize++;
    memcpy(&(buffer[0]), &tempSize, sizeof(tempSize));
}

gameObjectArray getArrayFromBuffer(char *buffer, int desiredIndex)
{
    gameObjectArray temp;

    //desiredIndex * sizeof(packet) essentially indexes through the buffer for each
    //packet inside the buffer
    memcpy(&temp, &(buffer[2 + (desiredIndex * sizeof(gameObjectArray))]), sizeof(gameObjectArray));

    return temp;
}

void addArrayToBuffer(char *buffer, gameObjectArray arrayVal, int *currentIndex)
{
    memcpy(&(buffer[2]), &arrayVal, sizeof(arrayVal));
    (*currentIndex)++;
    incrementBufferPacketArraySize(buffer);
    
}

int main()
{
    /*
    packet temp = {1, 8, 3, "Zach"};
    packet temp2 = {1,3,2, "Daniel"};

    short int size = 0;
    int index = 0;
    int desiredIndex = 1;

    printf("size is: %ld\n", sizeof(size));

    char buffer[sizeof(size) + sizeof(packet) * 2];

    //adding array size to beginning of buffer
    memcpy(&(buffer[0]), &size, sizeof(size));

    //adding data to buffer
    addPacketToBuffer(buffer, temp, &index);
    addPacketToBuffer(buffer, temp2, &index);

    //getting data out of the buffer
    packet newTemp;
    newTemp = getPacketFromBuffer(buffer, 0);
    printPacket(newTemp);
    //printf("%d\n", buffer[2]);
    short int tempVal;
    tempVal = getArraySize(buffer);
    printf("%d\n", tempVal);*/

    gameObject temp = {1, 2, 1, "X"};
    gameObject temp2 = {3, 4, 2, "Y"};

    gameObjectArray tempArr;
    initObjectArray(&tempArr, 2);

    addToObjArray(&tempArr, temp);
    addToObjArray(&tempArr, temp2);

    short int size = 0;
    int index = 0;

    char buffer[sizeof(size) + sizeof(tempArr)];


    buffer[0] = 1;
    
    addArrayToBuffer(buffer, tempArr, &index);
    
    gameObjectArray getArr;

    getArr = getArrayFromBuffer(buffer, 0);

    printf("%s\n", getArr.array[0].objectString); 


}