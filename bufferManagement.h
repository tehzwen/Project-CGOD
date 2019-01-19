#ifndef BUFFERMANAGEMENT_H
#define BUFFERMANAGEMENT_H
#include "packet.h"

int getArraySize(char *buffer);

void incrementBufferPacketArraySize(char *buffer);

packet getPacketFromBuffer(char *buffer, int desiredIndex);

void addPacketToBuffer(char *buffer, packet packVal, int *currentIndex);

void printPacket(packet packVal);

#endif