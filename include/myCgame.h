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



 struct entity{
    int x;
    int y; 
    char symbol;
    char obstructed_char;//No longer needed
} entity = {0, 0,FLOOR_TILE,FLOOR_TILE}; 

typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rectangle;

void draw_walls();
void update();
void handle_movement(struct entity* entity);
float* get_noise_map();
void draw_map(float* noiseMap, int size, Rectangle rect);
void draw_entity(struct entity* entity);
void draw_debug(struct entity* entity);
void move_tword_player(struct entity* entity,struct entity* player);
void move_entity(struct entity* entity,int x, int y);
static uint8_t astar_fill_callback(struct path_finder *path_finder, int32_t col, int32_t row);
void draw_path(struct path_finder* path_finder);
void do_astar(struct path_finder* path_finder,struct entity* start, struct entity* end);
struct path_finder init_astar(float* noise_map);

int map_array_size = MAP_HEIGHT * MAP_WIDTH * sizeof(float); 

