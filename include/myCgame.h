#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#define FNL_IMPL
#include "FastNoiseLite.h"
#include "uastar.h"

#define MAP_WIDTH 125
#define MAP_HEIGHT 125
#define FLOOR_TILE ' '
#define MOUNTAIN_TILE '^'
#define FENCE_TILE '#'
#define PATH_TILE '.'
#define HIDDEN_TILE ','
#define COLLISION_VALUE .2f



 struct entity{
    int x;
    int y; 
    char symbol;
} entity = {0, 0,FLOOR_TILE,}; 

typedef struct Vector2 {
    float x;                
    float y;                
} Vector2;

typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            
    float height;           
} Rectangle;

void init(fnl_state *); 
void update();
void handle_movement(struct entity* entity);
float* get_noise_map();
void draw_map(float* noiseMap, Rectangle* viewport);
void draw_entity(struct entity* entity);
void move_tword_player(struct entity* entity,struct entity* player);
void move_entity(struct entity* entity,int x, int y);
static uint8_t astar_fill_callback(struct path_finder *path_finder, int32_t col, int32_t row);
void draw_paths(struct path_finder* path_finder, Rectangle* viewport);
void do_astar(struct path_finder* path_finder,struct entity* start, struct entity* end);
struct path_finder init_astar(float* noise_map);
bool is_point_inside_rect(Rectangle* rect, int point_x, int point_y); 
    /* Raylib Functions*/
bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2);
bool CheckCollisionPointRec(Vector2 point, Rectangle rec);