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

#include "game.h"

#define PORT 3000

WINDOW *mainwin;
int oldcur;

void initObjArray(gameObject *objArray, int yMax, int xMax)
{
    for (int i = 0; i < 10; i++)
    {
        gameObject temp = {rand() % yMax, rand() % xMax, "(_)"};
        objArray[i] = temp;
    }
}

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

int runClient(void)
{

    //NETWORK CLIENT STUFF

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

    packet sendPack;
    sendPack.x = 0;
    sendPack.y = 0;
    strcpy(sendPack.userName, "boo");
    sendPack.active = 1;

    //END NETWORK CLIENT SETUP

    char ch;
    char gun = '-';
    int x = 0, y = 0;
    int yMax, xMax, startY, startX, gy, gx, direction;
    bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;

    gameObject rock = {10, 20, "(_)"};
    gameObject *objArray;

    getmaxyx(mainwin, yMax, xMax);

    startX = xMax / 2;
    startY = yMax / 2;
    gx = startX - 1;
    gy = startY;

    objArray = calloc(10, sizeof(gameObject));
    initObjArray(objArray, yMax, xMax);

    refreshScreen();

    while (1)
    {

        refreshScreen();
        move(startY, startX);
        printw("*");

        if (checkObjInbounds(rock, y, x, yMax, xMax))
            printObj(rock, y, x);

        for (int j = 0; j < 10; j++)
        {
            if (checkObjInbounds(objArray[j], y, x, yMax, xMax))
            {
                printObj(objArray[j], y, x);
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
            else
            {
                beep();
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

                packet temp = getPacketFromBuffer(receiveBuffer, 0);

                printw(temp.userName);
            }

            refresh();
            getch();
        }

        refreshScreen();
        clear();
        doupdate();
    }

    free(objArray);
    delwin(mainwin);
    endwin();

    return 0;
}

int main(void)
{
    initscr();

    mainwin = newwin(0, 0, 0, 0);
    oldcur = curs_set(0);

    runClient();

    return EXIT_SUCCESS;
}