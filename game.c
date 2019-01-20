#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>
#include <netinet/in.h>
#include "bufferManagement.h"
#include "serverGameLogic.h"
#include "game.h"
#include "gameObjectArray.h"

#define PORT 3000


WINDOW *mainwin;
int oldcur;

/*
void initObjectArray(gameObjectArray *a, size_t initialSize)
{
    a->size = initialSize;
    a->array = (gameObject *)malloc(initialSize * sizeof(gameObject));
    a->used = 0;
    a->currID = 0;
}

void addToObjArray(gameObjectArray *a, gameObject objVal)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (gameObject *)realloc(a->array, a->size * sizeof(gameObject));
    }
    a->array[a->used++] = objVal;
    a->currID++;
}

gameObject getGameObjectFromArray(gameObjectArray *a, int id)
{
    gameObject temp;

    for (int x = 0; x < a->used; x++)
    {

        if (a->array[x].id == id)
        {
            temp = a->array[x];
            return temp;
        }
    }

    return temp;
} */

void refreshScreen(void)
{
    box(mainwin, 0, 0);
    wrefresh(mainwin);
    refresh();
}

void movePlayer(int y, int x, char *val)
{
    move(y, x);
    printw("%s", val);
    refresh();
}

void checkObstacle(bool *up, bool *down, bool *left, bool *right, int y, int x)
{
    chtype upChar = mvinch(y - 2, x);
    chtype downChar = mvinch(y + 2, x);
    chtype leftChar = mvinch(y, x - 2);
    chtype rightChar = mvinch(y, x + 2);

    if (upChar == ' ')
        *up = true;
    else
        *up = false;
    if (downChar == ' ')
        *down = true;
    else
        *down = false;
    if (leftChar == ' ')
        *left = true;
    else
        *left = false;
    if (rightChar == ' ')
        *right = true;
    else
        *right = false;
}

bool checkObjInbounds(gameObject obj, int y, int x, int yMax, int xMax)
{
    if (obj.yCoord + y < yMax && obj.xCoord + x < xMax && obj.yCoord + y > 0 && obj.xCoord + x > 0)
        return true;

    return false;
}

void printObj(gameObject obj, int y, int x)
{
    movePlayer(obj.yCoord + y, obj.xCoord + x, obj.objectString);
}

int checkChar(char ch)
{
    if (ch == 'q')
        return QUIT;
    else if (ch == 'w' || ch == 65)
        return UP;
    else if (ch == 's' || ch == 66)
        return DOWN;
    else if (ch == 'a' || ch == 68)
        return LEFT;
    else if (ch == 'd' || ch == 67)
        return RIGHT;
    return -1;
}

int runClient(char *userName)
{

    //NETWORK CLIENT STUFF

    int sockfd;
    char receiveBuffer[sizeof(short int) + sizeof(packet) * 10];
    char hello[10];
    struct sockaddr_in servaddr;

    Array playerArray;
    initArray(&playerArray, 2);

    gameObjectArray objectArray;
    initObjectArray(&objectArray, 2);

    //creating border
    int testY = 0;
    int testX = 1;

    /*
    for (int x = 0; x < 150; x++)
    {
        gameObject tempLeft = {testY, 0, objectArray.currID, "<"};
        addToObjArray(&objectArray, tempLeft);
        gameObject tempRight = {testY, 300, objectArray.currID, ">"};
        addToObjArray(&objectArray, tempRight);

        testY++;
    }

    for (int x = 0; x < 300; x++)
    {
        
        gameObject tempTop = {150, testX, objectArray.currID, "_"};
        addToObjArray(&objectArray, tempTop);
        gameObject tempBottom = {0, testX, objectArray.currID, "^"};
        addToObjArray(&objectArray, tempBottom);

        testX++;
    }*/

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

    //END NETWORK CLIENT SETUP

    char ch;
    char gun = '-';
    int x = 0, y = 0;
    int yMax, xMax, startY, startX, gy, gx, direction;
    bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;

    getmaxyx(mainwin, yMax, xMax);

    startX = xMax / 2;
    startY = yMax / 2;
    gx = startX - 1;
    gy = startY;

    //initObjArray(objArray, yMax, xMax);

    refreshScreen();

    while (1)
    {

        packet sendPack;
        sendPack.x = x;
        sendPack.y = y;
        strcpy(sendPack.userName, userName);
        sendPack.active = 1;

        refreshScreen();
        move(startY, startX);
        printw("*");

        for (int j = 0; j < objectArray.used; j++)
        {
            if (checkObjInbounds(objectArray.array[j], y, x, yMax, xMax))
            {
                printObj(objectArray.array[j], y, x);
                refresh();
            }
        }
        checkObstacle(&canMoveUp, &canMoveDown, &canMoveLeft, &canMoveRight, startY, startX);
        move(gy, gx);
        printw("%c", gun);
        refresh();

        halfdelay(1);
        ch = getch();

        if (ch != ERR)
        {
            direction = checkChar(ch);

            if (direction == QUIT)
            {
                break;
            }
            else if (direction == UP && canMoveUp)
            {
                y++;
                gx = startX;
                gy = startY - 1;
                gun = '|';
            }
            else if (direction == DOWN && canMoveDown)
            {
                y--;
                gx = startX;
                gy = startY + 1;
                gun = '|';
            }
            else if (direction == LEFT && canMoveLeft)
            {
                x++;
                gx = startX - 1;
                gy = startY;
                gun = '-';
            }
            else if (direction == RIGHT && canMoveRight)
            {
                x--;
                gx = startX + 1;
                gy = startY;
                gun = '-';
            }
        }
        else
        {

            n = sendto(sockfd, (const packet *)&sendPack, sizeof(sendPack),
                       MSG_CONFIRM, (const struct sockaddr *)&servaddr,
                       sizeof(servaddr));

            if (n > 0)
            {
                n = recvfrom(sockfd, (char *)receiveBuffer, sizeof(receiveBuffer),
                             MSG_WAITALL, (struct sockaddr *)&servaddr,
                             &len);

                int arraySize = getArraySize(receiveBuffer);

                for (int i = 0; i < arraySize; i++)
                {
                    packet temp = getPacketFromBuffer(receiveBuffer, i);
                    if (!checkIfPlayerPacketExists(&playerArray, temp.userName))
                    {
                        /*move(temp.y + y, temp.x + x);
                        printw(temp.userName);
                            printf("%s vs %s\n", a->array[x].userName, userName);    refresh();*/
                        addToArray(&playerArray, temp);
                    }
                    else
                    {
                        updateClientInfo(&playerArray, temp);
                    }
                }

                for (int j = 0; j < playerArray.used; j++)
                {
                    move(playerArray.array[j].y, playerArray.array[j].x);
                    printw(playerArray.array[j].userName);
                }

                /*
                move(temp.y + y, temp.x + x);
                printw(temp.userName);
                refresh();*/
                refresh();
            }

            refresh();
            getch();
        }

        refreshScreen();
        clear();
        doupdate();
    }
    delwin(mainwin);
    endwin();

    return 0;
}

int main(int argc, char **argv)
{
    initscr();

    mainwin = newwin(0, 0, 0, 0);
    oldcur = curs_set(0);

    runClient(argv[argc - 1]);

    return EXIT_SUCCESS;
}