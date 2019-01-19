CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lncurses

all: game

game: game.c game.h
	$(CC) $(CFLAGS) -o game game.c $(LDFLAGS)

textClient: clientTest.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

textServer: serverTest.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bufferManagement: bufferManagement.c
	$(CC) $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

packetServer: bufferManagement.c packetServer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

packetClient: bufferManagement.c packetClient.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) *.o
	$(RM) game packetServer packetClient textClient textServer bufferManagement
