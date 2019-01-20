CC=gcc
CFLAGS=-Wall -g -Wno-unused-variable -pthread
LDFLAGS=-lncurses -lvlc

all: game

game: serverGameLogic.c bufferManagement.c game.c game.h
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

textClient: clientTest.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

textServer: serverTest.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bufferManagement: bufferManagement.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

packetServer: bufferManagement.c serverGameLogic.c packetServer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

packetClient: bufferManagement.c packetClient.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o
	$(RM) game packetServer packetClient textClient textServer bufferManagement
