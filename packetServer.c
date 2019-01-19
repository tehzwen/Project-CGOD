#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "bufferManagement.h"
#include <stdbool.h>

#define PORT 3000
#define MAXLINE 1024

typedef struct
{
    packet *array;
    size_t used;
    size_t size;

} Array;

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
    } else{

        return temp;
    }

    
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

void initArray(Array *a, size_t initialSize)
{
    a->size = initialSize;
    a->array = (packet *)malloc(initialSize * sizeof(packet));
    a->used = 0;
}

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[MAXLINE];
    //char *hostaddrp;
    //struct hostent *hostp;
    struct sockaddr_in servaddr, cliaddr;
    packet *receivePacket = calloc(0, sizeof(packet));

    //creates array size 10 to hold packets that are for
    //existing players on the server
    Array playerPacketArray;
    initArray(&playerPacketArray, 2);

    //creating sample packets and packet buffer
    char packetBuffer[sizeof(short int) + sizeof(packet) * 3];
    int packetBufferSize = 0;
    int packetBufferIndex = 0;

    //add array size to front of buffer
    memcpy(&(packetBuffer), &packetBufferSize, sizeof(packetBufferSize));


/*    
    packet newPack = {1, 1, 5, "zachary"};
    packet newPack2 = {1, 2, 10, "daniel"};
    packet newPack3 = {1, 3, 4, "dave"};
*/


    //test adding to packetArray
    /*addToArray(&playerPacketArray, newPack);
    packet Test1 = getPacketFromArray(&playerPacketArray, "zachary");
    printf("HERE: \n");
    printPacket(Test1);*/

    int tempVal = getArraySize(packetBuffer);
    printf("SIZE: %d\n", tempVal);

    /*
    addPacketToBuffer(packetBuffer, newPack, &packetBufferIndex);
    addPacketToBuffer(packetBuffer, newPack2, &packetBufferIndex);
    addPacketToBuffer(packetBuffer, newPack3, &packetBufferIndex);*/

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    socklen_t len;
    ssize_t n;

    while (1)
    {
        //receive string message from client
        n = recvfrom(sockfd, receivePacket, sizeof(*receivePacket),
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     &len);

        printf("%ld\n", n);
        //print string message from client
        //printf("server received %ld/%ld bytes\n", sizeof(receivePacket), n);
        if (n > 0)
        {
            printf("Received Packet!\n");
            printPacket(*receivePacket);
            if (!checkIfPlayerPacketExists(&playerPacketArray, receivePacket->userName)){
                addToArray(&playerPacketArray, *receivePacket);
            }
            else{
                //player exists, update their information and check for no cheating

            }
            
        }

        //send packetBuffer Array to client
        sendto(sockfd, (const char *)packetBuffer, sizeof(packetBuffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Hello message sent.\n");

        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
