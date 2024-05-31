#include <stdio.h>
#include <ncurses.h>

#define MAP_WIDTH 50
#define MAP_HEIGHT 20

void draw_walls();
void update();
void handle_movement(int* x, int* y);

int mains(){
       int num = 10; // Declare an integer variable
    int *ptr = &num; // Declare a pointer and assign the address of 'num' to it

    // Dereference the pointer to access the value
    printf("Value of num: %d\n", *ptr);
}




int main(){
    //init 
    initscr();
    cbreak();  
    //noecho();  
    keypad(stdscr, TRUE);  

    update(); // main game loop
    endwin();
    return 0;
}

void update(){
    int x = 10;
    int y = 10;
    while (true)
    {
        draw_walls();
        handle_movement(&x,&y);
        
        mvaddch(y,x,'@');
        refresh();
    }
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
}
void handle_movement(int *x, int *y) {
    int ch;
    int tempx,tempy;
    tempx = *x;
    tempy = *y;

    ch = getch();

    switch (ch) {
        case KEY_UP:
            mvaddch(*y,*x,' ');
            tempy--; 
            break;
        case KEY_DOWN:
            mvaddch(*y,*x,' ');
            tempy++;
            break;
        case KEY_LEFT:
            mvaddch(*y,*x,' ');
            tempx--;
            break;
        case KEY_RIGHT:
            mvaddch(*y,*x,' ');
            tempx++;
            break;
        }
    if (mvinch(tempy,tempx) == '#'){
        return;
    }
    else 
        *y = tempy;
        *x = tempx;    
}