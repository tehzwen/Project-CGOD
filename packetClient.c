#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "bufferManagement.h"

#define PORT 3000
#define MAXLINE 1024

//helper function for printing arrays containing packets
void printArray(char *receiveBuffer, int arrSize)
{

    for (int x = 0; x < arrSize; x++)
    {
        packet temp = getPacketFromBuffer(receiveBuffer, x);
        printPacket(temp);
    }
}

// Driver code
int main()
{
    int sockfd;
    char receiveBuffer[sizeof(short int) + sizeof(packet) * 10];
    char hello[10];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    //servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t len;
    ssize_t n;

    printf("Enter your username: ");
    scanf("%[^\n]s", hello);

    packet sendPack;
    sendPack.x = 0;
    sendPack.y = 0;
    strcpy(sendPack.userName, hello);
    sendPack.active = 1;

    sendto(sockfd, (const packet *)&sendPack, sizeof(sendPack),
           MSG_CONFIRM, (const struct sockaddr *)&servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)receiveBuffer, sizeof(receiveBuffer),
                 MSG_WAITALL, (struct sockaddr *)&servaddr,
                 &len);

    printf("%ld\n", n);

    printf("%d\n", receiveBuffer[0]);

    int arraySize = getArraySize(receiveBuffer);
    printf("SIZE: %d\n", arraySize);

    if (arraySize > 0)
    {
        printArray(receiveBuffer, arraySize);
    }

    //printf("Server : %x\n", buffer[3]);
    //printBuffer(buffer);

    memset(hello, 0, sizeof(hello));

    close(sockfd);
    return 0;
}
