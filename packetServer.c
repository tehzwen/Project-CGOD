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

#define PORT 3000
#define MAXLINE 1024

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[MAXLINE];
    //char *hostaddrp;
    //struct hostent *hostp;
    struct sockaddr_in servaddr, cliaddr;
    packet *receivePacket = malloc(sizeof(packet));

    //creating sample packets and packet buffer
    char packetBuffer[sizeof(short int) + sizeof(packet) * 3];
    int packetBufferSize = 0;
    int packetBufferIndex = 0;

    //add array size to front of buffer
    memcpy(&(packetBuffer), &packetBufferSize, sizeof(packetBufferSize));

    packet newPack = {1,1,5,"zachary"};
    packet newPack2 = {1,2,10, "daniel"};
    packet newPack3 = {1,3,4, "dave"};

    int tempVal = getArraySize(packetBuffer);
    printf("SIZE: %d\n", tempVal);

    addPacketToBuffer(packetBuffer, newPack, &packetBufferIndex);
    addPacketToBuffer(packetBuffer, newPack2, &packetBufferIndex);
    addPacketToBuffer(packetBuffer, newPack3, &packetBufferIndex);

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

        //print string message from client
        //printf("server received %ld/%ld bytes\n", sizeof(receivePacket), n);
        if (n > 0){
            printf("Received Packet!\n");
            printPacket(*receivePacket);

        }

        //send packetBuffer Array to client
        sendto(sockfd, (const char *)packetBuffer, sizeof(packetBuffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Hello message sent.\n");

        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}

/*

    while (1)
    {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     &len);

        //printf("N is equal to: %ld\n", n);

        buffer[n] = '\0';
        //printf("Client : %s\n", buffer);
        printf("server received %ld/%ld bytes: %s\n", strlen(buffer), n, buffer);

        struct sockaddr_in *pv4Addr = (struct sockaddr_in *)&cliaddr;
        struct in_addr ipAddr = pv4Addr->sin_addr;

        char ip[16];
        inet_ntop(AF_INET, &ipAddr, ip, sizeof(ip));
        printf("%s\n", ip);

        sendto(sockfd, (const packet*)&newPack, sizeof(newPack), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Hello message sent.\n");

        memset(buffer, 0, sizeof(buffer));
        //memset(ip, 0, sizeof(ip));
    }
*/