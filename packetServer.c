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
#include "gameObjectArray.h"
#include "serverGameLogic.h"
#include "mapBuffer.h"
#include <stdbool.h>

#define PORT 3000
#define MAXLINE 1024

//create buffer for map data, send it on first contact with client
//need to include way of creating objArrays here

// Initialize the server, returns the socket file descriptor of the binded port.
int init_server()
{

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("%d\n", sockfd);
    struct sockaddr_in servaddr;

    // Creating server socket file descriptor
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    int bind_ret = bind(sockfd, (const struct sockaddr *)&servaddr,
                        sizeof(servaddr));

    // Bind the socket with the server address
    if (bind_ret < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int main(int argc, char *argv[])
{

    int sockfd;
    char test[20] = "zach";
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

    int tempVal = getArraySize(packetBuffer);
    printf("SIZE: %d\n", tempVal);

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    int val = 1;
    int sockopt_ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    if (sockopt_ret < 0)
    {
        perror("Sock opt failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Bind the socket with the server address

    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //sockfd = init_server();

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

            //if the player doesn't exist then we append to the playerPacketArray
            if (!checkIfPlayerPacketExists(&playerPacketArray, receivePacket->userName))
            {

                addToArray(&playerPacketArray, *receivePacket);

                //create map buffer here

                //MAP STUFF
                gameObjectArray mapArray;
                initObjectArray(&mapArray, 2);

                int testY = 0;
                int testX = 1;

                gameObject topLeftStartPos = {10, 10, mapArray.currID, "&"};
                gameObject middleStartPos = {-50, -50, mapArray.currID, "&"};

                int randInt = rand() % (2 + 1 - 0) + 0;

                printf("RAND: %d\n", randInt);

                if (randInt == 0){
                    addToObjArray(&mapArray, middleStartPos);
                }

                else{
                    addToObjArray(&mapArray, topLeftStartPos);
                }
                

                

                //creating map buffer
                char mapBuffer[4 + (sizeof(gameObject) * mapArray.used)];

                short int objBufferSize = 0;
                short int mapStatus = 66;
                int objBufferIndex = 0;
                memcpy(&(mapBuffer[0]), &objBufferSize, sizeof(objBufferSize));
                memcpy(&(mapBuffer[2]), &mapStatus, sizeof(mapStatus));

                for (int x = 0; x < mapArray.used; x++)
                {
                    addObjectToBuffer(mapBuffer, mapArray.array[x], &objBufferIndex);
                }

                printf("size of the buffer%d\n", sizeof(mapBuffer));

                //END MAP STUFF

                //n = sendto(sockfd, (const char *)packetBuffer, sizeof(packetBuffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));

                n = sendto(sockfd, (const char *)mapBuffer, sizeof(mapBuffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));

                if (n > 0)
                {
                    printf("Sent confirmation message to client\n");
                }

                memset(packetBuffer, sizeof(packetBuffer), 1);
                packetBufferSize = 0;
                packetBufferIndex = 0;
                memcpy(&(packetBuffer), &packetBufferSize, sizeof(packetBufferSize));

                //packet replyLoginPacket = {3,0,0,"server"};
                //sendto(sockfd, (const char *)test, sizeof(test), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            }
            //if they do exist then we assume the game is running and this player
            //wants the information for all the other clients back
            else
            {
                updateClientInfo(&playerPacketArray, *receivePacket);

                for (int i = 0; i < playerPacketArray.used; i++)
                {

                    if (strcmp(playerPacketArray.array[i].userName, receivePacket->userName) != 0)
                    {
                        addPacketToBuffer(packetBuffer, playerPacketArray.array[i], &packetBufferIndex);
                    }
                }

                n = sendto(sockfd, (const char *)packetBuffer, sizeof(packetBuffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));

                if (n > 0)
                {
                    printf("Sent update fetch message to client\n");
                }

                memset(packetBuffer, sizeof(packetBuffer), 1);
                packetBufferSize = 0;
                packetBufferIndex = 0;
                memcpy(&(packetBuffer), &packetBufferSize, sizeof(packetBufferSize));

                //player exists, update their information and check for no cheating
            }
        }

        //print the playerarray if it is not empty

        if (playerPacketArray.used > 0)
        {
            printf("%s\n", playerPacketArray.array[0].userName);
        }

        //send packetBuffer Array to client
        //int y = sendto(sockfd, (const char *)packetBuffer, sizeof(packetBuffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, sizeof(cliaddr));
        //printf("Hello message sent %d.\n", y);
    }

    return 0;
}