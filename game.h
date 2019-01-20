#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include "gameObjectArray.h"

// Macros for moves
#define DOWN 1
#define UP 2
#define LEFT 3
#define RIGHT 4
#define QUIT 5

int runClient(char *userName);
void addToObjArray(gameObjectArray *a, gameObject objVal);
void refreshScreen(void);
int checkChar(char ch);
void movePlayer(int y, int x, char *val);
void initObjArray(gameObject *objArray, int yMax, int xMax);
bool checkObjInbounds(gameObject obj, int y, int x, int yMax, int xMax);
void checkObstacle(bool *up, bool *down, bool *left, bool *right, int y, int x);

#endif

