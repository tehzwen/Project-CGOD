#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <vlc/vlc.h>

typedef struct
{
    int yCoord;
    int xCoord;
    char *objectString;
} gameObject;

//takes boolean pointers and changes values depending on surroundings
void checkForObstacles(bool *up, bool *down, bool *left, bool *right, int y, int x)
{

    char upChar = mvinch(y - 2, x);
    char downChar = mvinch(y + 2, x);
    char leftChar = mvinch(y, x - 2);
    char rightChar = mvinch(y, x + 2);

    if (upChar == ' ')
    {
        *up = true;
    }
    else
    {
        *up = false;
    }
    if (downChar == ' ')
    {
        *down = true;
    }
    else
    {
        *down = false;
    }
    if (leftChar == ' ')
    {
        *left = true;
    }
    else
    {
        *left = false;
    }
    if (rightChar == ' ')
    {
        *right = true;
    }
    else
    {
        *right = false;
    }
}

void refreshScreen(WINDOW *win)
{
    box(win, 0, 0);
    wrefresh(win);
    refresh();
}

bool checkIfGameObjectIsInBounds(gameObject object, int y, int x, int yMax, int xMax)
{

    if (object.yCoord + y < yMax && object.xCoord + x < xMax && object.yCoord + y > 0 && object.xCoord + x > 0)
    {
        return true;
    }

    return false;
}

void moveToCoordAndPrintString(int y, int x, char *value)
{
    move(y, x);
    printw("%s", value);
    refresh();
}

//y and x are added to the gameObjects original location in the game
void printGameObjectToScreen(gameObject object, int y, int x)
{
    moveToCoordAndPrintString(object.yCoord + y, object.xCoord + x, object.objectString);
}

int main()
{

    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    inst = libvlc_new(0, NULL);
    m = libvlc_media_new_path(inst, "detective.wav");

    mp = libvlc_media_player_new_from_media(m);

    libvlc_media_release(m);

    libvlc_media_player_play(mp);

    WINDOW *mainwin;

    int width, height;
    bool canMoveUp, canMoveDown, canMoveLeft, canMoveRight;

    height = 50;
    width = 200;

    initscr();

    mainwin = newwin(height, width, 0, 0);

    int yMax, xMax;

    getmaxyx(mainwin, yMax, xMax);

    int startX = xMax / 2;
    int startY = yMax / 2;

    refreshScreen(mainwin);

    int x = 0;
    int y = 0;
    char gun = '-';
    int gx = startX - 1;
    int gy = startY;

    gameObject rock = {10, 20, "(_)"};
    gameObject *objectArray;

    objectArray = calloc(10, sizeof(gameObject));

    for (int i = 0; i < 10; i++)
    {
        gameObject temp = {5 + i, 4 + i, "(&)"};
        objectArray[i] = temp;
    }

    while (1)
    {
        refreshScreen(mainwin);
        curs_set(0);

        move(startY, startX);
        printw("*");

        if (checkIfGameObjectIsInBounds(rock, y, x, yMax, xMax))
        {
            printGameObjectToScreen(rock, y, x);
        }

        for (int j = 0; j < 10; j++)
        {
            if (checkIfGameObjectIsInBounds(objectArray[j], y, x, yMax, xMax))
            {
                printGameObjectToScreen(objectArray[j], y, x);
            }
        }
        refresh();

        checkForObstacles(&canMoveUp, &canMoveDown, &canMoveLeft, &canMoveRight, startY, startX);

        move(gy, gx);

        printw("%c", gun);
        refresh();
        
        char c = getch();


        clear();

        if (c == 'q')
        {
            break;
        }
        //move up
        else if (c == 'w' || c == 65)
        {

            if (canMoveUp)
            {
                y++;
                gx = startX;
                gy = startY - 1;
                gun = '|';
            }
            else
            {
                beep();
            }
        }
        //move down
        else if (c == 's' || c == 66)
        {

            if (canMoveDown)
            {
                y--;
                gx = startX;
                gy = startY + 1;
                gun = '|';
            }
            else
            {
                beep();
            }
        }

        //move left
        else if (c == 'a' || c == 68)
        {

            if (canMoveLeft)
            {
                x++;
                gx = startX - 1;
                gy = startY;
                gun = '-';
            }
            else
            {
                beep();
            }
        }

        //move right
        else if (c == 'd' || c == 67)
        {

            if (canMoveRight)
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

        refreshScreen(mainwin);
    }

    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);

    libvlc_release(inst);

    free(objectArray);
    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}