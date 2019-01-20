CC=gcc
CFLAGS=-Wall -g -Wno-unused-variable -pthread
LDFLAGS=-lncurses -lvlc

all: game

game: mapBuffer.c serverGameLogic.c gameObjectArray.c bufferManagement.c game.c game.h
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

textClient: clientTest.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

textServer: serverTest.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bufferManagement: bufferManagement.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

mapBuffer: mapBuffer.c gameObjectArray.c mapBuffer.h gameObjectArray.h
	$(CC) $(CFLAGS) -o mapBuffer mapBuffer.c gameObjectArray.c mapBuffer.h gameObjectArray.h

packetServer: mapBuffer.c bufferManagement.c gameObjectArray.c serverGameLogic.c packetServer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

packetClient: mapBuffer.c gameObjectArray.c bufferManagement.c packetClient.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o
	$(RM) game packetServer packetClient textClient textServer bufferManagement mapBuffer
