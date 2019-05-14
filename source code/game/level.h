#ifndef LEVEL_H
#define LEVEL_H

#include "bahamut.h"
#include "map.h"
#include "utils.h"

//
//   DATA TYPES
//

struct Enemy;
struct Bullet {
    vec2 pos;
    i32 timer;
    f32 angle;
    f32 speed;
    Texture img;
    Enemy* parent;
    void(*f)(Bullet* b); //function for bullet position
};

struct Emitter {
    i32 numBullets;
    f32 spread; //a measure of how spread out the spread shot will be
    i32 shootdelay;
    f32 speed;
    void(*f)(Bullet* b); //function for the algorithm
};

struct Enemy {
    Texture img;
    Texture bulletimg;
    vec2 pos;
    vec2 dest;
    i32 spawn;
    i32 stay;
    bool active;
    Emitter emitter;
};

static inline
vec2 calculate_seek(vec2 dest, Enemy* unit) {
    vec2 desired = MAX_SPEED * normalize(dest - unit->pos);
    return desired;
}

//
//   LEVEL
//

struct LevelScene;
#define MAX_BULLETS 5000
struct Level {
    Map map;
    std::vector<Enemy> enemies;
    std::vector<Unit> units;
    Bullet bullets[MAX_BULLETS];
    u16 numBullets;
    i32 timer;
    void(*init)(Level* level, LevelScene* scene);
};

struct LevelScene {
    Texture bigball;
    Texture ball;
    Texture bone;
    Texture candle;
    Texture skull;
    Texture tileset;
    Texture fairy;
    Texture unit;
    Texture skelly;
};

static inline
LevelScene load_level_scene() {
    LevelScene scene = {0};

    scene.bigball = load_texture("data/art/bigball.png", GL_NEAREST);
    scene.ball = load_texture("data/art/ball.png", GL_NEAREST);
    scene.skull = load_texture("data/art/skull.png", GL_NEAREST);
    scene.bone = load_texture("data/art/bone.png", GL_NEAREST);
    scene.candle = load_texture("data/art/candle.png", GL_NEAREST);
    scene.tileset = load_texture("data/art/tileset.png", GL_NEAREST);
    scene.fairy = load_texture("data/art/fairy.png", GL_NEAREST);
    scene.skelly = load_texture("data/art/skelly.png", GL_NEAREST);

    scene.unit = load_texture("data/art/unit.png", GL_NEAREST);

    return scene;
}

static inline
void draw_unit(RenderBatch* batch, Level* level, Unit* unit) {
    i32 unitx = (i32)unit->pos.x;
    i32 unity = (i32)unit->pos.y;

    if(unit->state == UNIT_WALKING) {
        if(unit->color == COLOR_BLUE) {
            update_animation(&unit->bluewalk, level->timer);
            draw_animation(batch, unit->bluewalk, unitx + level->map.x, unity + level->map.y, unit->right);
        }
        if(unit->color == COLOR_GREEN) {
            update_animation(&unit->greenwalk, level->timer);
            draw_animation(batch, unit->greenwalk, unitx + level->map.x, unity + level->map.y, unit->right);
        }
        if(unit->color == COLOR_YELLOW) {
            update_animation(&unit->yellowwalk, level->timer);
            draw_animation(batch, unit->yellowwalk, unitx + level->map.x, unity + level->map.y, unit->right);
        }
    }

    if(unit->state == UNIT_IDLE) {
        if(unit->color == COLOR_BLUE)
            draw_animation(batch, unit->blueidle, unitx + level->map.x, unity + level->map.y, unit->right);
        if(unit->color == COLOR_GREEN)
            draw_animation(batch, unit->greenidle, unitx + level->map.x, unity + level->map.y, unit->right);
        if(unit->color == COLOR_YELLOW)
            draw_animation(batch, unit->yellowidle, unitx + level->map.x, unity + level->map.y, unit->right);
    }

    if(unit->state == UNIT_JUMPING) {
        if(unit->color == COLOR_BLUE) {
            update_animation(&unit->bluejump, level->timer);
            draw_animation(batch, unit->bluejump, unitx + level->map.x, unity + level->map.y, unit->right);
        }
        if(unit->color == COLOR_GREEN) {
            update_animation(&unit->greenjump, level->timer);
            draw_animation(batch, unit->greenjump, unitx + level->map.x, unity + level->map.y, unit->right);
        }
        if(unit->color == COLOR_YELLOW) {
            update_animation(&unit->yellowjump, level->timer);
            draw_animation(batch, unit->yellowjump, unitx + level->map.x, unity + level->map.y, unit->right);
        }
    }

    if(unit->state == UNIT_DEAD) {
        if(unit->color == COLOR_BLUE) {
            update_animation(&unit->bluedead, level->timer);
            draw_animation(batch, unit->bluedead, unitx + level->map.x, unity + level->map.y, unit->right);
        }
        if(unit->color == COLOR_GREEN) {
            update_animation(&unit->greendead, level->timer);
            draw_animation(batch, unit->greendead, unitx + level->map.x, unity + level->map.y, unit->right);
        }
        if(unit->color == COLOR_YELLOW) {
            update_animation(&unit->yellowdead, level->timer);
            draw_animation(batch, unit->yellowdead, unitx + level->map.x, unity + level->map.y, unit->right);
        }
    }
}

static inline
void update_unit(Unit* unit, Level* level, Map* map) {
    unit->velocity.y += GRAVITY;

    if(unit->velocity.y > MAX_SPEED)
        unit->velocity.y = MAX_SPEED;

    i32 tilex = (i32)(unit->pos.x + unit->velocity.x) / TILE_SIZE;
    i32 tiley = (i32)(unit->pos.y + unit->velocity.y) / TILE_SIZE;

    bool leftcollide = unit->velocity.x > 0 && is_ground(map->grid[(tilex+1) + tiley * map->width]);
    bool rightcollide = unit->velocity.x < 0 && is_ground(map->grid[(tilex) + tiley * map->width]);

    if((is_ground(map->grid[tilex + (tiley+1) * map->width]) || is_floating_ground(map->grid[tilex + (tiley+1) * map->width]) || is_ground(map->grid[(tilex+1) + (tiley+1) * map->width]) || is_floating_ground(map->grid[(tilex+1) + (tiley+1) * map->width])) && unit->velocity.y > 0) {
        if(unit->state == UNIT_JUMPING)
            unit->state = UNIT_IDLE;
        unit->pos.y = (tiley * TILE_SIZE);
        unit->velocity.y = 0;

        if(unit->velocity.x != 0)
            unit->state = UNIT_WALKING;
    }
    if(map->grid[tilex + (tiley+1) * map->width] == 3 + 2 * TILESET_WIDTH) {
        unit->state = UNIT_DEAD;
        unit->velocity = {0};
    }

    if((is_ground(map->grid[tilex + (tiley) * map->width]) || is_ground(map->grid[(tilex+1) + (tiley) * map->width])) && unit->velocity.y < 0) {
        unit->velocity.y = 0;
    }

    if(leftcollide) {
        unit->pos.x = (tilex * TILE_SIZE);
    }
    else if(rightcollide) {
        unit->pos.x = ((tilex+1) * TILE_SIZE);
    } else
        unit->pos.x += unit->velocity.x;

    unit->pos.y += unit->velocity.y;

    //BULLET COLLISIONS
    for(u32 i = 0; i < level->numBullets; ++i) {
        if(getDistanceE(level->bullets[i].pos.x, level->bullets[i].pos.y, unit->pos.x, unit->pos.y) <= HITBOX && unit->state != UNIT_DEAD) {
            //swap the last bullet with this one then remove the last
            level->bullets[i] = level->bullets[level->numBullets-1];
            level->numBullets--;
            i--;
            unit->state = UNIT_DEAD;
            unit->velocity = {0};
        }
    }
}

static inline
void add_keypress(Unit* unit, i32 time, i32 key, bool press) {
    unit->keypresses.push_back({time, key, press});
}

static inline
void player_input(Unit* player, Level* level) {
    if(is_key_pressed(KEY_LEFT)) {
        if(player->state == UNIT_IDLE)
            player->state = UNIT_WALKING;
        add_keypress(player, level->timer, KEY_LEFT, true);
        player->right = false;
        player->velocity.x = -2;
    }
    if(is_key_released(KEY_LEFT)) {
        if(player->velocity.x < 0)
            player->velocity.x = 0;
        if(player->state == UNIT_WALKING)
            player->state = UNIT_IDLE;
        add_keypress(player, level->timer, KEY_LEFT, false);
    }
    if(is_key_pressed(KEY_RIGHT)) {
        if(player->state == UNIT_IDLE)
            player->state = UNIT_WALKING;
        add_keypress(player, level->timer, KEY_RIGHT, true);
        player->right = true;
        player->velocity.x = 2;
    }
    if(is_key_released(KEY_RIGHT)) {
        if(player->velocity.x > 0)
            player->velocity.x = 0;
        if(player->state == UNIT_WALKING)
            player->state = UNIT_IDLE;
        add_keypress(player, level->timer, KEY_RIGHT, false);
    }

    if(is_key_pressed(KEY_UP) || is_key_pressed(KEY_Z) || is_key_pressed(KEY_SPACE)) {
        if(player->state != UNIT_JUMPING) {
            level->units.back().velocity.y = -5;
            level->units.back().state = UNIT_JUMPING;
            add_keypress(player, level->timer, KEY_UP, true);
        }
    }
}

static inline
void ai_input(Unit* curr, i32 key, bool press) {
    if(key == KEY_LEFT) {
        if(press) {
            if(curr->state == UNIT_IDLE)
                curr->state = UNIT_WALKING;
            curr->right = false;
            curr->velocity.x = -2;
        } else {
            if(curr->velocity.x < 0)
                curr->velocity.x = 0;
            if(curr->state == UNIT_WALKING)
                curr->state = UNIT_IDLE;
        }
    }
    if(key == KEY_RIGHT) {
        if(press) {
            if(curr->state == UNIT_IDLE)
                curr->state = UNIT_WALKING;
            curr->right = true;
            curr->velocity.x = 2;
        } else {
            if(curr->velocity.x > 0)
                curr->velocity.x = 0;
            if(curr->state == UNIT_WALKING)
                curr->state = UNIT_IDLE;
        }
    }
    if(key == KEY_UP) {
        if(curr->state != UNIT_JUMPING) {
            curr->velocity.y = -5;
            curr->state = UNIT_JUMPING;
        }
    }
}

static inline
vec2 find_start_point(Level* level) {
    for(u32 x = 0; x < level->map.width; ++x) {
        for(u32 y = 0; y < level->map.height; ++y) {
            if(level->map.grid[x + y * level->map.width] == 4 + 1 * TILESET_WIDTH)
                return {(f32)x*TILE_SIZE, (f32)y*TILE_SIZE};
        }
    }
}

static inline
void reset_level(Level* level, LevelScene* scene) {
    level->numBullets = 0;
    level->enemies.clear();
    level->map.x = 0;
    level->map.y = -30;
    Unit unit = create_unit(scene->unit);
    unit.pos = find_start_point(level);

    level->units.push_back(unit);

    for(u32 i = 0; i < level->units.size(); ++i) {
        level->units[i].pos = {0};
        level->units[i].velocity = {0};
        level->units[i].force = {0};
        level->units[i].right = false;
        level->units[i].color = random_int(0, 100) > 50 ? COLOR_GREEN : COLOR_YELLOW;
        level->units[i].state = UNIT_IDLE;
        level->units[i].pos = find_start_point(level);
    }
    level->units.back().color = COLOR_BLUE;
    level->timer = 0;

}

static inline
void level(RenderBatch* batch, Level* level, LevelScene* scene) {
    draw_map(batch, &level->map, scene->tileset);
    level->timer++;

    if(level->units.size() == 0) {
        reset_level(level, scene);
        if(level->init != NULL)
            level->init(level, scene);
    }

    Unit* player = &level->units.back();
    if(player->state == UNIT_DEAD) {
        reset_level(level, scene);
        if(level->init != NULL)
            level->init(level, scene);
        return;
    }

    level->map.x -= 0.25f;

    for(u32 i = 0; i < level->enemies.size(); ++i) {
        Enemy* curr = &level->enemies[i];
        draw_texture(batch, curr->img, (i32)curr->pos.x + level->map.x, (i32)curr->pos.y + level->map.y);

        if(level->timer == curr->spawn) {
            curr->active = true;
        }
        if(curr->active) {
            if(curr->pos.y < -180) {
                level->enemies.erase(level->enemies.begin() + i);
                continue;
            }

            curr->stay--;
            if(curr->stay <= 0) {
                curr->dest = {curr->pos.x, -200};
            }
            if(getDistanceE(curr->pos.x, curr->pos.y, curr->dest.x, curr->dest.y) > 10) {
                curr->pos = curr->pos + calculate_seek(curr->dest, curr);
            }
            else {
                if(level->timer % curr->emitter.shootdelay == 0) {
                    f32 angle = 0;
                    for(u32 j = 0; j < curr->emitter.numBullets; ++j) {
                        Bullet bullet = {0};
                        bullet.pos = {curr->pos.x + (curr->img.width/2), curr->pos.y + (curr->img.height/2)};
                        bullet.img = curr->bulletimg;
                        bullet.f = curr->emitter.f;
                        bullet.angle = angle;
                        bullet.speed = curr->emitter.speed;
                        bullet.parent = curr;
                        angle += curr->emitter.spread;

                        if(level->numBullets < MAX_BULLETS) {
                            level->bullets[level->numBullets] = bullet;
                            level->numBullets++;
                        }
                    }
                }
            }
        }
    }

    player_input(player, level);

    for(u32 i = 0; i < level->units.size(); ++i) {
        Unit* unit = &level->units[i];
        draw_unit(batch, level, unit);
        update_unit(unit, level, &level->map);

        if(unit != player) {
            //every unit other than the player
            for(u32 j = 0; j < unit->keypresses.size(); ++j) {
                KeyPress* key = &unit->keypresses[j];
                if(level->timer == key->time) {
                    ai_input(unit, key->key, key->press);
                }
            }
        }
    }

    for(u32 i = 0; i < level->numBullets; ++i) {
        Bullet* curr = &level->bullets[i];
        curr->timer++;
        draw_texture(batch, curr->img, curr->pos.x + level->map.x, curr->pos.y + level->map.y);
        if(curr->parent->active && curr->f != NULL)
            curr->f(curr);
        curr->pos.x += cos(deg_to_rad(curr->angle))*curr->speed;
        curr->pos.y += sin(deg_to_rad(curr->angle))*curr->speed;

        if(curr->pos.x < -20 || curr->pos.y < -20 || curr->pos.x > level->map.width * TILE_SIZE || curr->pos.y > level->map.height * TILE_SIZE) {
            //swap the last bullet with this one then remove the last
            level->bullets[i] = level->bullets[level->numBullets-1];
            level->numBullets--;
            i--;
        }
    }
}

//
//   EDITOR
//

struct Editor {
    Map map;
    i32 selected;
};

static inline
void editor(RenderBatch* batch, Editor* editor, LevelScene* scene, vec2 mouse) {
    Map* map = &editor->map;

    if(is_key_released(KEY_F1))
        editor->selected = 0;
    if(is_key_released(KEY_F2))
        editor->selected = 1;
    if(is_key_released(KEY_F3))
        editor->selected = 2;
    if(is_key_released(KEY_F4))
        editor->selected = 3;
    if(is_key_released(KEY_F5))
        editor->selected = 4;
    if(is_key_released(KEY_F6))
        editor->selected = 5;
    if(is_key_released(KEY_F7))
        editor->selected = 6;
    if(is_key_released(KEY_F8))
        editor->selected = 7;
    if(is_key_released(KEY_F9))
        editor->selected = 8;
    if(is_key_released(KEY_F10))
        editor->selected = 9;
    if(is_key_released(KEY_F11))
        editor->selected = 10;
    if(is_key_released(KEY_ESCAPE))
        editor->selected = 11;

    if(is_key_released(KEY_F12)) {
        FILE *file = fopen("data/map.txt", "w");
        if (file == NULL)
            BMT_LOG(FATAL_ERROR, "Error opening file");

        fprintf(file, "#map\n");
        fprintf(file, "%d\n", editor->map.width);
        fprintf(file, "%d\n", editor->map.height);
        for (u16 i = 0; i < editor->map.width * editor->map.height; ++i)
            fprintf(file, "%d ", editor->map.grid[i]);
        fprintf(file, "\n");

        fclose(file);
    }

    if(is_key_released(KEY_C)) {
        editor->map = load_map("data/map.txt");
    }

    draw_rectangle(batch, 0, 0, 640, 360, BLACK);
    draw_map(batch, map, scene->tileset);

    if (is_key_down(KEY_LEFT))
        map->x += 8;
    if (is_key_down(KEY_RIGHT))
        map->x -= 8;
    if (is_key_down(KEY_DOWN))
        map->y -= 8;
    if (is_key_down(KEY_UP))
        map->y += 8;

    mouse.x -= map->x;
    mouse.y -= map->y;
    if(is_button_down(MOUSE_BUTTON_LEFT)) {
        if(mouse.x > 0 && mouse.y > 0 && mouse.x < map->width * TILE_SIZE && mouse.y < map->height * TILE_SIZE) {
            i32 tilex = mouse.x / TILE_SIZE;
            i32 tiley = mouse.y / TILE_SIZE;
            //add tiles based on selected type, then orient ground tiles
            if(editor->selected == 0) {
                map->grid[tilex + tiley * map->width] = 0;
            }
            if(editor->selected == 1)
                map->grid[tilex + tiley * map->width] = 0 + 3 * TILESET_WIDTH;
            if(editor->selected == 2)
                map->grid[tilex + tiley * map->width] = 1 + 3 * TILESET_WIDTH;
            if(editor->selected == 3)
                map->grid[tilex + tiley * map->width] = 4;
            if(editor->selected == 4)
                map->grid[tilex + tiley * map->width] = 4 + 1 * TILESET_WIDTH;
            if(editor->selected == 5)
                map->grid[tilex + tiley * map->width] = 4 + 2 * TILESET_WIDTH;
            if(editor->selected == 6)
                map->grid[tilex + tiley * map->width] = 3 + 2 * TILESET_WIDTH;
            if(editor->selected == 7)
                map->grid[tilex + tiley * map->width] = 0 + 4 * TILESET_WIDTH;
            if(editor->selected == 8)
                map->grid[tilex + tiley * map->width] = 1 + 4 * TILESET_WIDTH;
            if(editor->selected == 9)
                map->grid[tilex + tiley * map->width] = 4 + 4 * TILESET_WIDTH;
            if(editor->selected == 10)
                map->grid[tilex + tiley * map->width] = 4 + 2 * TILESET_WIDTH;
            if(editor->selected == 11)
                map->grid[tilex + tiley * map->width] = 2 + 4 * TILESET_WIDTH;

            orient_tiles(map);
        }
    }
}

#endif
