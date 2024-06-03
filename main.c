#include "include/myCgame.h"
#include "include/uastar.h"
#include <ncurses.h>
#include <time.h>

fnl_state noise_source;
float* noise_map;
struct path_finder path_finder; 

int main(){
    init(&noise_source);
    noise_map = get_noise_map();
    
    update(); 
    
    endwin();
    free(noise_map);
    return 0;
}

void init(fnl_state *noise){
        /* Init Curses */
    initscr(); //must be first
    start_color();
    curs_set(0); //disable cursor
    cbreak();  
    noecho();  
    keypad(stdscr, TRUE); //enable keys for main screen
    //nodelay(stdscr, TRUE); 
    halfdelay(10); //using this to limit fps, do it correctly!

    //create color bindings
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); 
    init_pair(3, COLOR_GREEN, COLOR_BLACK); 

    /* Init FastNoise */
    *noise = fnlCreateState();
    noise->noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise->frequency = .1f;
}

void update(){
    Rectangle viewport;
    //int max_x,max_y;
    //getmaxyx(stdscr, max_y, max_x);
    
    struct entity player = {19,30, 'P',FLOOR_TILE}; 
    struct entity enemy = {12,9, 'E',FLOOR_TILE};                 
                viewport = (Rectangle){0,0,MAP_WIDTH,MAP_HEIGHT}; //DEBUG, show whole screen
    path_finder = init_astar(noise_map);
    while (true){
        /*
            UPDATE
        */
        //viewport = (Rectangle){player.x-19,player.y-10,40,40};  // causing crazy issues

        handle_movement(&player);

        if (is_point_inside_rect(&viewport,enemy.x, enemy.y)) {
            do_astar(&path_finder,&enemy, &player);
        }

        /*
            RENDER
        */
        erase(); // May not be needed?
        draw_map(noise_map, map_array_size,viewport);
        draw_entity(&player);
        draw_entity(&enemy);
        draw_path(&path_finder);

        refresh();
    }
}


/*
    Game Functions
*/
/*
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
*/
void handle_movement(struct entity* entity) {
    int ch;
    int tempx,tempy;
    
    ch = getch(); 

    if (ch == ERR) { return; }

    tempx = entity->x;
    tempy = entity->y;
    
    switch (ch) {
        case KEY_UP:
            tempy--; 
            break;
        case KEY_DOWN:
            tempy++;
            break;
        case KEY_LEFT:
            tempx--;
            break;
        case KEY_RIGHT:
            tempx++;
            break;
    }
        // Immovable object collision 
    float noise_value = noise_map[(tempy * MAP_WIDTH) + tempx]; 
    if (noise_value >= COLLISION_VALUE){
        return;
    }
    else{
        move_entity(entity,tempx,tempy);  
    } 
}

float* get_noise_map(){
    //int max_x,max_y;
    //getmaxyx(stdscr, max_y, max_x);
    float* noiseData = malloc(MAP_WIDTH * MAP_HEIGHT * sizeof(float)); // need to do this, dynamic 
    int index = 0;
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++) 
        {
            noiseData[index++] = fnlGetNoise2D(&noise_source, (float) x, (float) y); // if you divide by bax, image is distorted
        }
    }
    return noiseData;
}

void draw_map(float* noiseMap, int size, Rectangle viewport){
    int width_x = (int)viewport.width;
    for (int y = viewport.y; y <= viewport.y+viewport.height; y++) {
        for (int x = viewport.x; x <= viewport.x+viewport.width; x++) {
            float noiseValue = noise_map[(y * width_x) + x]; 
            if (noiseValue >= COLLISION_VALUE) {
                attron(COLOR_PAIR(2));
                mvaddch(y, x, MOUNTAIN_TILE);
                attroff(COLOR_PAIR(2));
            }
            else {
                //attron(COLOR_PAIR(3));
                mvaddch(y, x, FLOOR_TILE);
                //attroff(COLOR_PAIR(3));
            }
        }
    }
}

void draw_entity(struct entity* entity){
    attron(COLOR_PAIR(1));
    mvaddch(entity->y,entity->x,entity->symbol);
    attroff(COLOR_PAIR(1));
}

void move_tword_player(struct entity* entity,struct entity* player){
    // this will move the entity towrds the player using astar*
}

void move_entity(struct entity* entity,int x, int y){
    entity->y = y;
    entity->x = x;   
}

static uint8_t astar_fill_callback(struct path_finder *path_finder, int32_t col, int32_t row){
	uint8_t is_passable = 0;
    float* noise_map = path_finder->data;
    float noise_value = noise_map[(row * MAP_WIDTH) + col]; 

    if (noise_value <= COLLISION_VALUE)  {
		is_passable = 1;
	}
	return is_passable;
}

void draw_path(struct path_finder* path_finder){
    //int max_x,max_y;
    //getmaxyx(stdscr, max_y, max_x);
    if (path_finder == NULL) {return;}

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            if (path_finder_is_path(path_finder, x, y)) {
                attron(COLOR_PAIR(3));
                mvaddch(y, x, PATH_TILE);
                attroff(COLOR_PAIR(3));
            }
        }
    }
}

void do_astar(struct path_finder* path_finder,struct entity* start, struct entity* end){
    if (path_finder == NULL) { return; }
    path_finder_clear_path(path_finder);
    path_finder_set_start(path_finder, start->x, start->y);
    path_finder_set_end(path_finder, end->x, end->y);
    path_finder_find(path_finder,NULL);
}

struct path_finder init_astar(float* noise_map){
    struct path_finder path_finder = {0};
    path_finder_initialize(&path_finder);
    path_finder.cols = MAP_WIDTH; 
    path_finder.rows = MAP_HEIGHT;
    path_finder.fill_func = astar_fill_callback; 
    path_finder.score_func = NULL;
    path_finder.data = noise_map;
    path_finder_fill(&path_finder);
    return path_finder;
}
bool is_point_inside_rect(Rectangle* rect, int point_x, int point_y) {
  return (point_x > rect->x && point_x < rect->x + rect->width &&
          point_y > rect->y && point_y < rect->y + rect->height);
}
