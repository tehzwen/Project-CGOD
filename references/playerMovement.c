#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

int main(){

    WINDOW *mainwin;


    mainwin = initscr();
    int x = 10;
    int y = 10;
    char gun = '-';
    int gx = 11;
    int gy = 10;

    while (1){
        curs_set(0);
        
        
        move (y,x);
        printw("*");
        //mvprintw(y+5, x+5, "%c", gun);
        move(gy,gx);
        
        printw("%c", gun);
        refresh(); 

        char c = getch();
        clear();

        move(0,0);
        printw("%d", c);

        if (c == 'q'){
            break;
        }
        //move up
        else if (c == 'w' || c == 65){
            y--;
            gx = x;
            gy = y - 1;
            gun = '|';
        }
        //move down
        else if (c == 's' || c == 66){
            y++;
            gx = x;
            gy = y + 1;
            gun = '|';
        }

        //move left
        else if (c == 'a' || c == 68){
            x--;
            gx = x - 1;
            gy = y;
            gun = '-';
        }

        //move right
        else if (c == 'd' || c == 67){
            x++;
            gx = x + 1;
            gy = y;
            gun = '-';

        }

    }

    delwin(mainwin);
    endwin();
    refresh();
    

    

    return EXIT_SUCCESS;
}