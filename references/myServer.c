#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];
    char reply[2000] = "hey there";

    //creating the socket here
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1){
        printf("Could not create socket!");
    }

    puts("Socket created!");

    //prepare the struct 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //binding socket here

    if (bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0){
        perror("bind Failed. Error");
        return 1;
    }
    puts ("Bind done");

    //listening

    listen(socket_desc, 3);

    //accept incoming connections

    puts("Waiting for incoming connections....");

    c = sizeof(struct sockaddr_in);

    //accept cnnection from client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

    if (client_sock < 0){
        perror("accept failed");
        return 1;
    }

    puts("Connection accepted");

    //receive from client
    while((read_size = recv(client_sock, client_message, 2000, 0)) > 0){

        printf("Received message: %s\n", client_message);
        memset(client_message, 0, sizeof(client_message));

        //send the message back to the client
        //write(client_sock, client_message, strlen(client_message));
        write(client_sock, reply, strlen(reply));
    }

    if (read_size == 0){
        puts ("Client disconnected!");
        fflush(stdout);
    }
    else if (read_size == -1){
        perror("recv failed");
    }

    close(client_sock);
    close(socket_desc);

    return 0;

}