#include <stdlib.h>
#include <ncurses.h>

#define MAP_WIDTH 50
#define MAP_HEIGHT 20

void draw_walls();


int main(){
    initscr();
    cbreak();  
    noecho();  



    draw_walls();






    getch(); 
    endwin();
    return 0;
}

void draw_walls(){
    for (int x = 0; x < MAP_WIDTH; x++){
        for (int y = 0; y < MAP_HEIGHT; y++){
            if (y == 0 || y == MAP_HEIGHT-1)
            {
                mvaddch(y,x,'#');
            }
            if (x == 0 || x == MAP_WIDTH-1)
            {
                mvaddch(y,x,'#');
            }
            


            
        }
    }
    refresh();
}