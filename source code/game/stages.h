#ifndef STAGES_H
#define STAGES_H

#include "level.h"

//
//   BULLET PATTERNS
//

static inline
void spread_countercircle(Bullet* b) {

}

static inline
void spread(Bullet* b) {

}

static inline
void spiral(Bullet* b) {
    if(b->timer < 380)
        b->angle+=0.55f;
}

//
//   ADD STAGE ENEMIES
//

static inline
void stage1(Level* level, LevelScene* scene) {
    Enemy enemy = {0};
    enemy.img = scene->fairy;
    enemy.spawn = 10;
    enemy.dest = {100, 100};
    enemy.pos = {150, -100};
    enemy.stay = 500;
    enemy.bulletimg = scene->ball;
    enemy.emitter = {5, 360.0f/5.0f, 100, 1, spiral};
    level->enemies.push_back(enemy);
    enemy.dest = {140, 100};
    enemy.stay = 600;
    enemy.bulletimg = scene->ball;
    enemy.emitter = {10, 360.0f/10.0f, 70, 2, spiral};
    level->enemies.push_back(enemy);
    enemy.dest = {180, 100};
    enemy.stay = 650;
    enemy.bulletimg = scene->bigball;
    enemy.emitter = {2, 360.0f/4.0f, 100, 1, spiral};
    level->enemies.push_back(enemy);

    enemy.img = scene->skelly;
    enemy.spawn = 350;
    enemy.dest = {650, 80};
    enemy.stay = 950;
    enemy.bulletimg = scene->bone;
    enemy.emitter = {20, 360.0f/20.0f, 100, 1.5, spiral};
    level->enemies.push_back(enemy);

    enemy.img = scene->fairy;
    enemy.spawn = 650;
    enemy.dest = {750, 150};
    enemy.pos = {650, -150};
    enemy.stay = 500;
    enemy.bulletimg = scene->ball;
    enemy.emitter = {10, 360.0f/5.0f, 100, 0.5, spiral};
    level->enemies.push_back(enemy);
    enemy.dest = {780, 150};
    level->enemies.push_back(enemy);
    enemy.dest = {810, 150};
    level->enemies.push_back(enemy);

    enemy.img = scene->fairy;
    enemy.spawn = 1320;
    enemy.dest = {820, 290};
    enemy.pos = {950, -150};
    enemy.stay = 500;
    enemy.bulletimg = scene->ball;
    enemy.emitter = {10, 360.0f/5.0f, 100, 0.3f, spiral};
    level->enemies.push_back(enemy);
    enemy.dest = {780, 150};
    level->enemies.push_back(enemy);
    enemy.dest = {810, 150};
    level->enemies.push_back(enemy);

    enemy.img = scene->skull;
    enemy.spawn = 1670;
    enemy.dest = {1020, 150};
    enemy.pos = {950, -50};
    enemy.stay = 150;
    enemy.bulletimg = scene->bone;
    enemy.emitter = {2, 360.0f/5.0f, 100, 1.3f, spiral};
    level->enemies.push_back(enemy);
}

static inline
void stage2(Level* level, LevelScene* scene) {

}

#endif
