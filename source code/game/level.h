#ifndef LEVEL_H
#define LEVEL_H

#include "bahamut.h"
#include "map.h"
#include "utils.h"

//
//   DATA TYPES
//

struct Enemy;
struct Emitter;
struct Bullet {
    vec2 pos;
    i32 timer;
    f32 angle;
    f32 speed;
    Texture img;
    Enemy* parent;
    void(*f)(Bullet* b, Emitter* e, i32 i); //function for bullet position
};

struct Emitter {
    i32 numBullets;
    f32 spread; //a measure of how spread out the spread shot will be
    f32 angle;
    i32 shootdelay;
    f32 speed;
    void(*f)(Bullet* b, Emitter* e, i32 i); //function for the algorithm
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

struct Explosion {
    vec2 pos;
    f32 scale;
};

enum ScrollDir {
    SCROLL_LEFT,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN,
    SCROLL_NONE
};

struct LevelScene;
#define MAX_BULLETS 1000
struct Level {
    Map map;
    bool pause;
    bool dead;
    bool victory;
    ScrollDir scroll;
    std::vector<Enemy> enemies;
    std::vector<Unit> units;
    std::vector<Explosion> explosions;
    Bullet bullets[MAX_BULLETS];
    u16 numBullets;
    i32 timer;
    void(*init)(Level* level, LevelScene* scene);
};

struct LevelScene {
    Texture bigball;
    Texture ball;
    Texture redball;
    Texture bone;
    Texture candle;
    Texture skull;
    Texture tileset;
    Texture fairy;
    Texture unit;
    Texture skelly;
    Texture explosion;
    Texture heart;
    Texture angel;
    BitmapFont font;

    Sound bullet;
};

static inline
LevelScene load_level_scene() {
    LevelScene scene = {0};

    scene.bigball = load_texture("data/art/bigball.png", GL_NEAREST);
    scene.ball = load_texture("data/art/ball.png", GL_NEAREST);
    scene.redball = load_texture("data/art/ball_red.png", GL_NEAREST);
    scene.skull = load_texture("data/art/skull.png", GL_NEAREST);
    scene.bone = load_texture("data/art/bone.png", GL_NEAREST);
    scene.candle = load_texture("data/art/candle.png", GL_NEAREST);
    scene.tileset = load_texture("data/art/tileset.png", GL_NEAREST);
    scene.fairy = load_texture("data/art/fairy.png", GL_NEAREST);
    scene.skelly = load_texture("data/art/skelly.png", GL_NEAREST);
    scene.explosion = load_texture("data/art/explosion.png", GL_NEAREST);
    scene.heart = load_texture("data/art/heart.png", GL_NEAREST);
    scene.angel = load_texture("data/art/angel.png", GL_NEAREST);

    scene.unit = load_texture("data/art/unit.png", GL_NEAREST);

    scene.bullet = load_sound("data/sound/bullet.wav");
    scene.font = load_1bit_font();

    return scene;
}

static inline
void draw_unit(RenderBatch* batch, Level* level, LevelScene* scene, Unit* unit) {
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

    if(unit->slow) 
        draw_texture(batch, scene->heart, unitx + (i32)(TILE_SIZE/2) - (i32)(scene->heart.width/2) + level->map.x, unity + (i32)(TILE_SIZE/2) + level->map.y);
}

static inline
void explode(Unit* unit, Level* level) {
    level->explosions.push_back({{unit->pos.x, unit->pos.y}, 0});
    for(u32 i = 0; i < level->numBullets; ++i) {
        if(getDistanceE((level->bullets[i].pos.x + level->bullets[i].img.width/2), (level->bullets[i].pos.y + level->bullets[i].img.height/2), (unit->pos.x + TILE_SIZE/2), (unit->pos.y + TILE_SIZE/2)) <= 64*2) {
            level->bullets[i] = level->bullets[level->numBullets-1];
            level->numBullets--;
            i--;
        }
    }
}

static inline
void update_unit(Unit* unit, Level* level, Map* map, LevelScene* scene) {
    if(unit->falltime > 0)
        unit->falltime--;

    if(unit->slow)
        unit->velocity.y += SLOW_GRAVITY;
    else
        unit->velocity.y += GRAVITY;

    if(unit->velocity.y > MAX_SPEED)
        unit->velocity.y = MAX_SPEED;

    i32 tilex = (i32)(unit->pos.x) / TILE_SIZE;
    i32 tiley = (i32)(unit->pos.y + unit->velocity.y) / TILE_SIZE;
    i32 tilex2 =(i32)(unit->pos.x+TILE_SIZE-2) / TILE_SIZE;
    i32 tiley2 =(i32)(unit->pos.y+TILE_SIZE-2) / TILE_SIZE;

    bool leftcollide = unit->velocity.x > 0 && is_ground(map->grid[(tilex+1) + tiley * map->width]);
    bool rightcollide = unit->velocity.x < 0 && is_ground(map->grid[(tilex) + tiley * map->width]);

    if((is_ground(map->grid[tilex + (tiley) * map->width]) || is_ground(map->grid[tilex2 + (tiley) * map->width])) && unit->velocity.y < 0) {
        unit->pos.y += 1;
        unit->velocity.y = 0;
    }

    bool fallingThrough = false;
    if(unit->falltime > 0) {
        fallingThrough = true;
    }
    if((is_ground(map->grid[tilex + (tiley+1) * map->width]) || is_ground(map->grid[tilex2 + (tiley+1) * map->width]) || is_floating_ground(map->grid[tilex + (tiley+1) * map->width]) || is_floating_ground(map->grid[(tilex+1) + (tiley+1) * map->width])) && unit->velocity.y > 0 && !fallingThrough) {
        if(unit->state == UNIT_JUMPING)
            unit->state = UNIT_IDLE;
        unit->pos.y = (tiley * TILE_SIZE);
        unit->velocity.y = 0;

        if(unit->velocity.x != 0)
            unit->state = UNIT_WALKING;
    }
    if(map->grid[tilex + (tiley) * map->width] == 3 + 2 * TILESET_WIDTH && unit->state != UNIT_DEAD) {
        unit->state = UNIT_DEAD;
        unit->velocity = {0};
        if(unit == &level->units.back())
            level->pause = true;
        explode(unit, level);
    }

    if(unit->state != UNIT_DEAD) {
        if(leftcollide) {
            unit->pos.x = (tilex * TILE_SIZE);
        }
        else if(rightcollide) {
            unit->pos.x = ((tilex+1) * TILE_SIZE);
        }
        else
            unit->pos.x += unit->velocity.x;
    }


    unit->pos.y += unit->velocity.y;

    //BULLET COLLISIONS
    for(u32 i = 0; i < level->numBullets; ++i) {
        if(getDistanceE((level->bullets[i].pos.x + level->bullets[i].img.width/2), (level->bullets[i].pos.y + level->bullets[i].img.height/2), (unit->pos.x + TILE_SIZE/2), (unit->pos.y + TILE_SIZE/2)) <= HITBOX && unit->state != UNIT_DEAD) {
            //swap the last bullet with this one then remove the last
            explode(unit, level);
            unit->state = UNIT_DEAD;
            unit->velocity = {0};
            if(unit == &level->units.back())
                level->pause = true;
            break;
        }
    }
}

static inline
void add_keypress(Unit* unit, i32 time, i32 key, bool press) {
    unit->keypresses.push_back({time, key, press});
}

static inline
void ai_input(Unit* curr, Level* level, i32 key, bool press) {
    if(curr->state == UNIT_DEAD) 
        return;

    if(key == KEY_LEFT) {
        if(press) {
            if(curr->state == UNIT_IDLE)
                curr->state = UNIT_WALKING;
            curr->right = false;
            curr->velocity.x = curr->slow ? -1 : -2;
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
            curr->velocity.x = curr->slow ? 1 : 2;
        } else {
            if(curr->velocity.x > 0)
                curr->velocity.x = 0;
            if(curr->state == UNIT_WALKING)
                curr->state = UNIT_IDLE;
        }
    }
    if(key == KEY_UP) {
        if(curr->state != UNIT_JUMPING) {
            curr->velocity.y = curr->slow ? -3 : -5;
            curr->state = UNIT_JUMPING;
        }
    }
    if(key == KEY_DOWN) {
        curr->falltime = 15;
        if(curr->slow)
            curr->falltime = 20;
    }
    if(key == KEY_LEFT_SHIFT) {
        if(press) {
            if(curr->velocity.x < 0)
                curr->velocity.x = -1;
            else if(curr->velocity.x > 0)
                curr->velocity.x = 1;

            curr->slow = true;
        } else {
            if(curr->velocity.x < 0)
                curr->velocity.x = -2;
            else if(curr->velocity.x > 0)
                curr->velocity.x = 2;
            curr->slow = false;
        }
    }
    if(key == KEY_C) {
        if(curr->bombs > 0) {
            explode(curr, level);
            curr->bombs--;
            curr->flytime = 500;
            //curr->state = UNIT_FLYING;
        }
    }
    if(key == KEY_ESCAPE)
        curr->state = UNIT_DEAD;
}

static inline
void player_input(Unit* player, Level* level) {
    if(is_key_pressed(KEY_LEFT)) {
        ai_input(player, level, KEY_LEFT, true);
        add_keypress(player, level->timer, KEY_LEFT, true);
    }
    if(is_key_released(KEY_LEFT)) {
        ai_input(player, level, KEY_LEFT, false);
        add_keypress(player, level->timer, KEY_LEFT, false);
    }
    if(is_key_pressed(KEY_RIGHT)) {
        ai_input(player, level, KEY_RIGHT, true);
        add_keypress(player, level->timer, KEY_RIGHT, true);
    }
    if(is_key_released(KEY_RIGHT)) {
        ai_input(player, level, KEY_RIGHT, false);
        add_keypress(player, level->timer, KEY_RIGHT, false);
    }

    if(is_key_pressed(KEY_UP) || is_key_pressed(KEY_Z) || is_key_pressed(KEY_SPACE)) {
        if(player->state != UNIT_JUMPING) {
            ai_input(player, level, KEY_UP, true);
            add_keypress(player, level->timer, KEY_UP, true);
        }
    }

    i32 tilex = (i32)(player->pos.x) / TILE_SIZE;
    i32 tiley = (i32)(player->pos.y) / TILE_SIZE;
    if(is_key_pressed(KEY_DOWN)) {
        if(is_floating_ground(level->map.grid[(tilex+1) + (tiley+1) * level->map.width]) || is_floating_ground(level->map.grid[tilex + (tiley+1) * level->map.width])) {
            ai_input(player, level, KEY_DOWN, true);
            add_keypress(player, level->timer, KEY_DOWN, true);
        }
    }
    if(is_key_pressed(KEY_LEFT_SHIFT)) {
        ai_input(player, level, KEY_LEFT_SHIFT, true);
        add_keypress(player, level->timer, KEY_LEFT_SHIFT, true);
    }
    if(is_key_released(KEY_LEFT_SHIFT)) {
        ai_input(player, level, KEY_LEFT_SHIFT, false);
        add_keypress(player, level->timer, KEY_LEFT_SHIFT, false);
    }
    if(is_key_pressed(KEY_C)) {
        if(player->bombs > 0) {
            ai_input(player, level, KEY_C, true);
            add_keypress(player, level->timer, KEY_C, true);
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
    level->explosions.clear();
    level->map.x = 0;
    level->map.y = -30;
    Unit unit = create_unit(scene->unit);
    unit.pos = find_start_point(level);

    level->units.push_back(unit);

    for(u32 i = 0; i < level->units.size(); ++i) {
        level->units[i].pos = {0};
        level->units[i].velocity = {0};
        level->units[i].force = {0};
        level->units[i].slow = false;
        level->units[i].bombs = 1;
        level->units[i].right = false;
        level->units[i].color = random_int(0, 100) > 50 ? COLOR_GREEN : COLOR_YELLOW;
        level->units[i].state = UNIT_IDLE;
        level->units[i].pos = find_start_point(level);
    }
    level->units.back().color = COLOR_BLUE;
    level->timer = 0;

}

static inline
void level(RenderBatch* batch, Level* level, LevelScene* scene, i32* currlevel) {
    if(level->units.size() == 0) {
        reset_level(level, scene);
        if(level->init != NULL)
            level->init(level, scene);
    }

    draw_map(batch, &level->map, scene->tileset);
    Unit* player = &level->units.back();

    if(is_key_released(KEY_P) || is_key_released(KEY_ESCAPE)) {
        level->pause = true;
        return;
    }
    if(level->pause) {
        for(u32 i = 0; i < level->enemies.size(); ++i) {
            Enemy* curr = &level->enemies[i];
            draw_texture(batch, curr->img, (i32)curr->pos.x + level->map.x, (i32)curr->pos.y + level->map.y);
        }
        for(u32 i = 0; i < level->explosions.size(); ++i) {
            level->explosions[i].scale += 0.1f;
            scene->explosion.width *= level->explosions[i].scale;
            scene->explosion.height *= level->explosions[i].scale;
            draw_texture(batch, scene->explosion, level->explosions[i].pos.x + (TILE_SIZE/2) - ((scene->explosion.width)/2) + level->map.x, level->explosions[i].pos.y + (TILE_SIZE/2) - ((scene->explosion.height)/2) + level->map.y, {1, 1, 1, 1-(level->explosions[i].scale/3)});
            scene->explosion.width = 64;
            scene->explosion.height = 64;

            if(level->explosions[i].scale >= 3) {
                level->explosions.erase(level->explosions.begin() + i);
                i--;
                continue;
            }
        }
        for(u32 i = 0; i < level->numBullets; ++i) {
            Bullet* curr = &level->bullets[i];
            draw_texture(batch, curr->img, curr->pos.x + level->map.x, curr->pos.y + level->map.y);
            if(curr->parent->active && curr->f != NULL)
                curr->f(curr, &curr->parent->emitter, i);
            if(curr->pos.x < -20 || curr->pos.y < -20 || curr->pos.x > level->map.width * TILE_SIZE || curr->pos.y > level->map.height * TILE_SIZE) {
                //swap the last bullet with this one then remove the last
                level->bullets[i] = level->bullets[level->numBullets-1];
                level->numBullets--;
                i--;
            }
        }
        if(player->state == UNIT_DEAD) {
            draw_text(batch, &scene->font, "YOU DIED", 300, 120);
            draw_text(batch, &scene->font, "PRESS E TO CONTINUE", 200, 160);
            //draw_text
            if(is_key_released(KEY_E)) {
                level->pause = false;
            }
            if(is_key_released(KEY_R)) {
                level->pause = false;
                add_keypress(player, level->timer, KEY_ESCAPE, false);
                reset_level(level, scene);
                if(level->init != NULL)
                    level->init(level, scene);
                return;
            }

        }
        else if(level->victory) {
            draw_text(batch, &scene->font, "VICTORY", 300, 120);
            draw_text(batch, &scene->font, "PRESS E TO CONTINUE", 200, 160);

            if(is_key_released(KEY_E)) {
                level->pause = false;
                *currlevel = *currlevel + 1;
            }
        } else {
            draw_text(batch, &scene->font, "PAUSED", 300, 120);
            draw_text(batch, &scene->font, "PRESS R TO SUICIDE", 200, 160);
            draw_text(batch, &scene->font, "PRESS E TO CONTINUE", 200, 190);
            draw_text(batch, &scene->font, "PRESS M TO MUTE SOUNDS", 150, 220);
            draw_text(batch, &scene->font, "PRESS N TO UNMUTE SOUNDS", 140, 250);
            //draw_text
            if(is_key_released(KEY_M)) {
                set_master_volume(0);
            }
            if(is_key_released(KEY_N)) {
                set_master_volume(110);
            }
            if(is_key_released(KEY_E)) {
                level->pause = false;
            }
            if(is_key_released(KEY_R)) {
                level->pause = false;
                add_keypress(player, level->timer, KEY_ESCAPE, false);
                reset_level(level, scene);
                if(level->init != NULL)
                    level->init(level, scene);
                return;
            }
        }
    } else {

    level->timer++;

    if(player->state == UNIT_DEAD && level->pause == false) {
        add_keypress(player, level->timer, KEY_ESCAPE, false);
        reset_level(level, scene);
        if(level->init != NULL)
            level->init(level, scene);
        return;
    }

    if(level->scroll == SCROLL_RIGHT)
        level->map.x -= 0.25f;
    if(level->scroll == SCROLL_LEFT)
        level->map.x += 0.25f;
    if(level->scroll == SCROLL_DOWN)
        level->map.y -= 0.25f;

    if(player->pos.x < -level->map.x - 50 || player->pos.y < -level->map.y - 50 || player->pos.y > -level->map.y + get_virtual_height()-0) {
        player->state = UNIT_DEAD;
        level->pause = true;
    }

    i32 tilex = (i32)(player->pos.x) / TILE_SIZE;
    i32 tiley = (i32)(player->pos.y + player->velocity.y) / TILE_SIZE;
    if(level->map.grid[tilex + tiley * level->map.width] == 4 + 2 * TILESET_WIDTH) {
        //level has been won
        level->pause = true;
        level->victory = true;
    }

    for(u32 i = 0; i < level->enemies.size(); ++i) {
        Enemy* curr = &level->enemies[i];
        draw_texture(batch, curr->img, (i32)curr->pos.x + level->map.x, (i32)curr->pos.y + level->map.y);

        if(level->timer == curr->spawn) {
            curr->active = true;
            if(i == level->enemies.size() - 1 && curr->img.ID == 0 && player->state != UNIT_DEAD)
                *currlevel = *currlevel + 1;
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
                    f32 angle = curr->emitter.angle;
                    play_sound(scene->bullet);
                    for(u32 j = 0; j < curr->emitter.numBullets; ++j) {
                        Bullet bullet = {0};
                        bullet.img = curr->bulletimg;
                        bullet.pos = {curr->pos.x + (curr->img.width/2) - (bullet.img.width/2), curr->pos.y + (curr->img.height/2) - (bullet.img.height/2)};
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

    if(player->state != UNIT_DEAD)
        player_input(player, level);

    for(u32 i = 0; i < level->explosions.size(); ++i) {
        level->explosions[i].scale += 0.1f;
        scene->explosion.width *= level->explosions[i].scale;
        scene->explosion.height *= level->explosions[i].scale;
        draw_texture(batch, scene->explosion, level->explosions[i].pos.x + (TILE_SIZE/2) - ((scene->explosion.width)/2) + level->map.x, level->explosions[i].pos.y + (TILE_SIZE/2) - ((scene->explosion.height)/2) + level->map.y, {1, 1, 1, 1-(level->explosions[i].scale/3)});
        scene->explosion.width = 64;
        scene->explosion.height = 64;

        if(level->explosions[i].scale >= 3) {
            level->explosions.erase(level->explosions.begin() + i);
            i--;
            continue;
        }
    }

    for(u32 i = 0; i < level->units.size(); ++i) {
        Unit* unit = &level->units[i];
        draw_unit(batch, level, scene, unit);
        update_unit(unit, level, &level->map, scene);

        if(unit != player) {
            //every unit other than the player
            for(u32 j = 0; j < unit->keypresses.size(); ++j) {
                KeyPress* key = &unit->keypresses[j];
                if(level->timer == key->time) {
                    ai_input(unit, level, key->key, key->press);
                }
            }
        }
    }

    for(u32 i = 0; i < level->numBullets; ++i) {
        Bullet* curr = &level->bullets[i];
        curr->timer++;
        draw_texture(batch, curr->img, curr->pos.x + level->map.x, curr->pos.y + level->map.y);
        if(curr->parent->active && curr->f != NULL)
            curr->f(curr, &curr->parent->emitter, i);
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
