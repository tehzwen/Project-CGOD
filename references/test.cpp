#include <ncurses.h>
using namespace std;


//compile with g++ -lncurses test.cpp -o hello
int main(int argc, char ** argv){

    initscr();

    refresh();

    getch();

    refresh();


    return 0;
}