#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000
#define MAXLINE 1024

// Driver code
int main()
{
    int sockfd;
    char buffer[MAXLINE];
    char *hostaddrp;
    struct hostent *hostp;
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

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

        sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Hello message sent.\n");

        memset(buffer, 0, sizeof(buffer));
        //memset(ip, 0, sizeof(ip));
    }

    return 0;
}