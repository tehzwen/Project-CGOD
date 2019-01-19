
CFLAGS = -Wall -std=c99
CC = gcc

all: game

game:
	$(CC) $(CFLAGS) -o game game.c -lncurses -lvlc

textClient:
	gcc -o textClient clientTest.c 

textServer:
	gcc -o textServer serverTest.c 

bufferManagement: bufferManagement.c
	$(CC) $(CFLAGS) -c bufferManagement.c

packetServer: bufferManagement.c
	$(CC) $(CFLAGS) -o packetServer packetServer.c bufferManagement.c

packetClient: bufferManagement.c
	$(CC) $(CFLAGS) -o packetClient packetClient.c bufferManagement.c

clean:
	rm *.o
	rm packetServer
	rm packetClient
