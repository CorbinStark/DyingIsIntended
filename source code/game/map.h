#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>
#include <algorithm>
#include "bahamut.h"
#include "utils.h"

//
//   CONSTANTS
//

const i32 TILE_SIZE = 16;
const i32 TILESET_WIDTH = 5;
const i32 SCROLL_SPEED = 4;

const f32 HITBOX = 7;
const f32 GRAVITY = 0.25;
const f32 MAX_SPEED = 2;
const f32 MAX_FORCE = 1.0;
const f32 SCALING_FACTOR = (1/15.0f);

//
//   DATA TYPES
//

enum UnitState {
    UNIT_IDLE,
    UNIT_WALKING,
    UNIT_JUMPING,
    UNIT_DEAD
};

struct KeyPress {
    i32 time;
    i32 key;
    bool press;
};

enum UnitColor {
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_YELLOW
};

struct Unit {
    vec2 pos;
    vec2 velocity;
    vec2 force;
    std::vector<KeyPress> keypresses; //save key presses for future runs
    UnitState state;
    UnitColor color;
    bool right;

    Animation blueidle;
    Animation bluewalk;
    Animation bluejump;
    Animation bluedead;

    Animation yellowidle;
    Animation yellowwalk;
    Animation yellowjump;
    Animation yellowdead;

    Animation greenidle;
    Animation greenwalk;
    Animation greenjump;
    Animation greendead;
};

struct Map {
    i32* grid;
    u16 width;
    u16 height;
    f32 x;
    f32 y;
};

//
//   FUNCTIONS
//

static inline
Unit create_unit(Texture unitsheet) {
    Unit unit = {0};
    unit.state = UNIT_IDLE;

    unit.blueidle = create_animation(unitsheet, 1, 0, 16, 16, 15);
    unit.bluewalk = create_animation(unitsheet, 2, 0, 16, 16, 15);
    unit.bluejump = create_animation(unitsheet, 3, 16, 16, 16, 15);
    unit.bluedead = create_animation(unitsheet, 1, 32, 16, 16, 15);

    unit.yellowidle = create_animation(unitsheet, 1, 48, 16, 16, 15);
    unit.yellowwalk = create_animation(unitsheet, 2, 48, 16, 16, 15);
    unit.yellowjump = create_animation(unitsheet, 3, 64, 16, 16, 15);
    unit.yellowdead = create_animation(unitsheet, 1, 80, 16, 16, 15);

    unit.greenidle = create_animation(unitsheet, 1, 96, 16, 16, 15);
    unit.greenwalk = create_animation(unitsheet, 2, 96, 16, 16, 15);
    unit.greenjump = create_animation(unitsheet, 3, 112, 16, 16, 15);
    unit.greendead = create_animation(unitsheet, 1, 128, 16, 16, 15);

    return unit;
}

static inline
bool is_ground(i32 tile) {
    return tile % TILESET_WIDTH < 3 && tile / TILESET_WIDTH < 3;
}

static inline
bool is_floating_ground(i32 tile) {
    return tile % TILESET_WIDTH > 0 && tile % TILESET_WIDTH < 4 && tile / TILESET_WIDTH == 3;
}

static inline
void check_ground_neighbors(Map* map, i32 x, i32 y, bool* up, bool* down, bool* left, bool* right) {
    *up = true;
    *down = true;
    *left = true;
    *right = true;

    if(y-1 >= 0) 
        *up = is_ground(map->grid[x + (y-1) * map->width]);
    if(y + 1 <= map->height)
        *down = is_ground(map->grid[x + (y+1) * map->width]);
    if(x - 1 >= 0)
        *left = is_ground(map->grid[(x-1) + y * map->width]);
    if(x + 1 <= map->width)
        *right = is_ground(map->grid[(x+1) + y * map->width]);
}

static inline
void orient_tiles(Map* map) {
    for(i32 x = 0; x < map->width; ++x) {
        for(i32 y = 0; y < map->height; y++) {
            u32 index = x + y * map->width;
            i32 tile = map->grid[x + y * map->width];
            bool left, right, up, down;
            check_ground_neighbors(map, x, y, &up, &down, &left, &right);

            //ground
            if(is_ground(tile)) {
                if(right && down)
                    map->grid[index] = 0;
                if(left && down)
                    map->grid[index] = 2;
                if(right && up)
                    map->grid[index] = 0 + 2 * TILESET_WIDTH;
                if(left && up)
                    map->grid[index] = 2 + 2 * TILESET_WIDTH;

                if(left && right) {
                    if(!up)
                        map->grid[index] = 1;
                    if(!down)
                        map->grid[index] = 1 + 2 * TILESET_WIDTH;
                }
                if(up && down) {
                    if(!left)
                        map->grid[index] = 0 + 1 * TILESET_WIDTH;
                    if(!right)
                        map->grid[index] = 2 + 1 * TILESET_WIDTH;
                }
                if(up && down && left && right)
                    map->grid[index] = 1 + 1 * TILESET_WIDTH;

            }
            //floating ground
            if(is_floating_ground(tile)) {
                map->grid[x + y * map->width] = 2 + 3 * TILESET_WIDTH;
                if(x - 1 >= 0 && !is_floating_ground(map->grid[(x - 1) + y * map->width]))
                    map->grid[x + y * map->width] = 1 + 3 * TILESET_WIDTH;
                if(x + 1 < map->width && !is_floating_ground(map->grid[(x + 1) + y * map->width]))
                    map->grid[x + y * map->width] = 3 + 3 * TILESET_WIDTH;
            }
        }
    }
}

static inline
void truncate(vec2* v, f32 max) {
    f32 i = 0;
    i = max / length(*v);
    i = i < 1.0 ? i : 1.0;
    v->x *= i;
    v->y *= i;
}

static inline
void clamp(i32* in, i32 min, i32 max) {
    if(*in > max) *in = max;
    if(*in < min) *in = min;
}

static inline
void clamp(f32* in, f32 min, f32 max) {
    if(*in > max) *in = max;
    if(*in < min) *in = min;
}

static inline
Map create_map(i32 width, i32 height) {
    Map map = {0};

    map.width = width;
    map.height = height;
    map.grid = (i32*)malloc(sizeof(i32) * width * height);
    for(u32 i = 0; i < width*height; ++i)
        map.grid[i] = 22;

    return map;
}

static inline
Map load_map(const char* filename) {
    Map map = { 0 };

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        BMT_LOG(FATAL_ERROR, "Error opening file");

    char buffer[255];
    fgets(buffer, 255, file);

    i32 width;
    i32 height;
    fscanf(file, "%d", &width);
    fscanf(file, "%d", &height);
    map = create_map(width, height);
    for (u16 i = 0; i < width * height; ++i) {
        i32 id;
        fscanf(file, "%d", &id);
        map.grid[i] = id;
    }

    fclose(file);

    return map;
}

static inline
void draw_map(RenderBatch* batch, Map* map, Texture tileset) {
    i32 x0 = ((i32)-map->x / TILE_SIZE);
    i32 x1 = ((i32)-map->x / TILE_SIZE) + (get_virtual_width() / TILE_SIZE) + 1;
    i32 y0 = ((i32)-map->y / TILE_SIZE);
    i32 y1 = ((i32)-map->y / TILE_SIZE) + (get_virtual_height() / TILE_SIZE) + 2;

    clamp(&x0, 0, map->width);
    clamp(&x1, 0, map->width);
    clamp(&y0, 0, map->height);
    clamp(&y1, 0, map->height);

    Rect dest = {0, 0, (f32)TILE_SIZE, (f32)TILE_SIZE};
    Rect src = {0, 0, (f32)TILE_SIZE, (f32)TILE_SIZE};

    for(u32 x = x0; x < x1; ++x) {
        for(u32 y = y0; y < y1; ++y) {
            i32 id = map->grid[x + y * map->width];

            dest.x = (x * TILE_SIZE) + map->x;
            dest.y = (y * TILE_SIZE) + map->y;
            src.x = (id % TILESET_WIDTH) * TILE_SIZE;
            src.y = (id / TILESET_WIDTH) * TILE_SIZE;

            draw_texture_EX(batch, tileset, src, dest);
        }
    }
}

static inline
void dispose_map(Map* map) {
    free(map->grid);
}

#endif
