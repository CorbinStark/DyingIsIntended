#ifndef STAGES_H
#define STAGES_H

#include "level.h"

//
//   BULLET PATTERNS
//

static inline
void spread_countercircle(Bullet* b, Emitter* e, i32 i) {

}

static inline
void twirl(Bullet* b, Emitter* e, i32 i) {
    e->angle -= 0.05f;
    e->spread += 0.0001f;
    b->angle += 0.01f;
    b->speed += 0.0001f;
}

static inline
void spread(Bullet* b, Emitter* e, i32 i) {
    if(b->timer > 400)
        b->speed += 0.002f;
}

static inline
void expanding_shield(Bullet* b, Emitter* e, i32 i) {
    if(b->timer < 530) {
        if(b->timer % 400 < 200)
            b->angle += 2.40f;
        else
            b->angle -= 2.40f;
    }

    if(b->timer > 500)
        b->speed += 0.004f;
}

static inline
void expanding_shield_reverse(Bullet* b, Emitter* e, i32 i) {
    if(b->timer < 530) {
        if(b->timer % 400 < 200)
            b->angle -= 2.40f;
        else
            b->angle += 2.40f;
    }

    if(b->timer > 500)
        b->speed += 0.004f;
}

static inline 
void criss_cross(Bullet* b, Emitter* e, i32 i) {
    if(i % 2 == 1)
        b->angle += 0.35f;
    else
        b->angle -= 0.35f;
}

static inline
void spiral(Bullet* b, Emitter* e, i32 i) {
    if(b->timer < 380)
        b->angle+=0.55f;
    if(b->timer > 400)
        b->speed += 0.002f;
}

//
//   ADD STAGE ENEMIES
//

static inline
void populate_enemy(Enemy* enemy, Texture img, i32 spawn, vec2 dest, vec2 pos, i32 stay, Texture bulletimg) {
    enemy->img = img;
    enemy->spawn = spawn;
    enemy->dest = dest;
    enemy->pos = pos;
    enemy->stay = stay;
    enemy->bulletimg = bulletimg;
}

static inline
void stage1(Level* level, LevelScene* scene) {
    level->map.x = 50;
    Enemy enemy = {0};

    populate_enemy(&enemy, scene->fairy, 1, {100, 100}, {150, -100}, 500, scene->ball);
    enemy.emitter = {10, 90.0f/9.0f, 0, 100, 1, criss_cross};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 10, {140, 100}, {150, -100}, 600, scene->ball);
    enemy.emitter = {10, 360.0f/10.0f, 0, 70, 2, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 10, {180, 100}, {150, -100}, 650, scene->bigball);
    enemy.emitter = {3, 360.0f/4.0f, 0, 100, 1, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skelly, 350, {650, 80}, {150, -100}, 950, scene->bone);
    enemy.emitter = {20, 360.0f/20.0f, 0, 100, 1.5, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skelly, 350, {650, 80}, {150, -100}, 950, scene->bone);
    enemy.emitter = {20, 360.0f/20.0f, 0, 80, 1.7f, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 650, {750, 150}, {650, -150}, 500, scene->ball);
    enemy.emitter = {10, 360.0f/5.0f, 0, 100, 0.5, spiral};
    level->enemies.push_back(enemy);
    enemy.dest = {780, 150};
    level->enemies.push_back(enemy);
    enemy.dest = {810, 150};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1320, {820, 290}, {950, -150}, 500, scene->ball);
    enemy.emitter = {7, 360.0f/5.0f, 0, 100, 0.8f, spiral};
    level->enemies.push_back(enemy);
    enemy.dest = {780, 150};
    level->enemies.push_back(enemy);
    enemy.dest = {810, 150};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 2370, {1020, 150}, {950, -50}, 750, scene->ball);
    enemy.emitter = {26, 360.0f/26.0f, 0, 250, 0.8f, expanding_shield};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->skull, 2370, {1020, 150}, {950, -50}, 750, scene->ball);
    enemy.emitter = {26, 360.0f/26.0f, 0, 500, 0.8f, expanding_shield_reverse};
    level->enemies.push_back(enemy);
    enemy.bulletimg = scene->bone;
    enemy.stay = 950;
    enemy.emitter = {26, 360.0f/26.0f, 0, 80, 0.8f, spread};
    level->enemies.push_back(enemy);
    enemy.emitter = {30, 360.0f/30.0f, 0, 120, 0.7f, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skelly, 3900, {1500, 150}, {1400, -55}, 1400, scene->bone);
    enemy.emitter = {35, 360.0f/35.0f, 0, 510, 0.45f, spread};
    level->enemies.push_back(enemy);
    enemy.emitter = {5, 360.0f/5.0f, 0, 20, 0.8f, twirl};
    level->enemies.push_back(enemy);
    enemy.emitter = {5, 360.0f/5.0f, 0, 110, 1.1f, spiral};
    level->enemies.push_back(enemy);
    enemy.emitter = {10, 360.0f/12.0f, 0, 220, 0.8f, spiral};
    level->enemies.push_back(enemy);
}

static inline
void stage2(Level* level, LevelScene* scene) {

}

#endif
