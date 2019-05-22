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
void twirl_reverse(Bullet* b, Emitter* e, i32 i) {
    e->angle += 0.05f;
    e->spread -= 0.0001f;
    b->angle -= 0.01f;
    b->speed += 0.0001f;
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
void spread_rotate(Bullet* b, Emitter* e, i32 i) {
    e->angle += 1/60.0f;
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
void stage0(Level* level, LevelScene* scene) {
    Enemy enemy = {0};

    populate_enemy(&enemy, scene->skull, 1, {170, 180}, {130, 0}, 200, scene->bone);
    enemy.emitter = {20, 360.0f/20.0f, 50, 50, 1, expanding_shield};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 150, {150, 150}, {130, 0}, 100, scene->ball);
    enemy.emitter = {3, 90.0f/5.0f, 50, 80, 1, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 160, {170, 150}, {130, 0}, 140, scene->ball);
    enemy.emitter = {3, 90.0f/5.0f, 80, 50, 1, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 170, {190, 150}, {130, 0}, 140, scene->ball);
    enemy.emitter = {3, 90.0f/5.0f, 80, 50, 1, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 180, {120, 150}, {130, 0}, 140, scene->ball);
    enemy.emitter = {3, 90.0f/5.0f, 50, 50, 1, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 185, {50, 150}, {130, 0}, 150, scene->ball);
    enemy.emitter = {3, 90.0f/5.0f, 50, 50, 1, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 240, {350, 80}, {290, 0}, 500, scene->ball);
    enemy.emitter = {15, 360.0f/15.0f, 0, 120, 1.6f, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 550, {550, 40}, {590, 0}, 700, scene->bigball);
    enemy.emitter = {20, 360.0f/20, 0, 80, 1.0f, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 750, {450, 140}, {590, 0}, 700, scene->bone);
    enemy.emitter = {20, 360.0f/20, 0, 80, 1.0f, twirl};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1060, {550, 40}, {590, 0}, 500, scene->ball);
    enemy.emitter = {15, 360.0f/15.0f, 0, 120, 1.6f, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1090, {550, 280}, {490, 0}, 500, scene->ball);
    enemy.emitter = {15, 360.0f/15.0f, 0, 120, 1.6f, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 1050, {550, 280}, {490, 0}, 200, scene->bone);
    enemy.emitter = {25, 360.0f/25.0f, 0, 120, 1.0f, expanding_shield};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 2050, {450, 130}, {490, 0}, 200, scene->bone);
    enemy.emitter = {35, 360.0f/35.0f, 0, 50, 1.0f, spiral};
    level->enemies.push_back(enemy);
}

static inline
void stage1(Level* level, LevelScene* scene) {
    Enemy enemy = {0};

    populate_enemy(&enemy, scene->skull, 1, {370, 130}, {430, 0}, 800, scene->bone);
    enemy.emitter = {10, 360.0f/10.0f, 50, 50, 1, twirl};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 1, {370, 130}, {430, 0}, 800, scene->ball);
    enemy.emitter = {20, 360.0f/20.0f, 50, 70, 1, twirl_reverse};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1050, {550, 170}, {430, 0}, 400, scene->ball);
    enemy.emitter = {20, 360.0f/20.0f, 50, 70, 1, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1150, {710, 40}, {430, 0}, 400, scene->bigball);
    enemy.emitter = {20, 360.0f/20.0f, 50, 70, 1.2f, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1911, {1170, 80}, {430, 0}, 800, scene->ball);
    enemy.emitter = {20, 360.0f/20.0f, 50, 150, 1, twirl};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 1911, {1170, 260}, {430, 0}, 800, scene->bone);
    enemy.emitter = {10, 360.0f/10.0f, 50, 150, 1, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 1211, {980, 180}, {430, 0}, 800, scene->bone);
    enemy.emitter = {3, 360.0f/3.0f, 50, 15, 1, spread_rotate};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 2650, {1550, 40}, {590, 0}, 1700, scene->bigball);
    enemy.emitter = {20, 360.0f/20, 0, 80, 1.0f, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 2650, {1450, 140}, {590, 0}, 1700, scene->bone);
    enemy.emitter = {20, 360.0f/20, 0, 80, 1.0f, twirl};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 2660, {1550, 40}, {590, 0}, 1500, scene->ball);
    enemy.emitter = {15, 360.0f/15.0f, 0, 120, 1.6f, spiral};
    level->enemies.push_back(enemy);
}

static inline
void test_stage(Level* level, LevelScene* scene) {
    Enemy enemy = {0};

    populate_enemy(&enemy, scene->skull, 1211, {980, 120}, {430, 0}, 800, scene->bone);
    enemy.emitter = {3, 360.0f/3.0f, 50, 5, 1, spread_rotate};
    level->enemies.push_back(enemy);
}

static inline
void stage2(Level* level, LevelScene* scene) {
    level->map.x = 50;
    Enemy enemy = {0};

    populate_enemy(&enemy, scene->fairy, 1, {100, 100}, {150, -100}, 500, scene->ball);
    enemy.emitter = {10, 90.0f/9.0f, 0, 100, 1, criss_cross};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 10, {140, 100}, {150, -100}, 600, scene->redball);
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
void stage3(Level* level, LevelScene* scene) {
    Enemy enemy = {0};

    populate_enemy(&enemy, scene->fairy, 50, {150, 180}, {150, 15}, 800, scene->ball);
    enemy.emitter = {4, 90, 0, 5, 3.5, spread};
    level->enemies.push_back(enemy);
    enemy.dest = {250, 180};
    level->enemies.push_back(enemy);
    enemy.dest = {350, 180};
    level->enemies.push_back(enemy);
    enemy.dest = {450, 180};
    level->enemies.push_back(enemy);
    enemy.dest = {550, 180};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skelly, 170, {300, 260}, {150, -100}, 950, scene->bone);
    enemy.emitter = {40, 360.0f/40.0f, 0, 80, 1.7f, spread};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->skelly, 170, {300, 260}, {150, -100}, 950, scene->bone);
    enemy.emitter = {10, 160.0f/10.0f, 0, 80, 1.7f, twirl};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->skelly, 170, {300, 260}, {150, -100}, 950, scene->ball);
    enemy.emitter = {10, 360.0f/10.0f, 0, 80, 1.7f, expanding_shield};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 1550, {50, 680}, {70, 155}, 800, scene->ball);
    enemy.emitter = {4, 90, 0, 10, 3.0, spread};
    level->enemies.push_back(enemy);
    enemy.dest = {50, 780};
    level->enemies.push_back(enemy);
    enemy.dest = {50, 880};
    level->enemies.push_back(enemy);
    enemy.dest = {450, 980};
    level->enemies.push_back(enemy);
    enemy.dest = {450, 1080};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->fairy, 1550, {330, 720}, {70, 155}, 800, scene->bigball);
    enemy.dest = {330, 720};
    enemy.emitter = {12, 360.0f/12.0f, 0, 70, .8f, spread};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->skull, 1550, {330, 720}, {70, 155}, 800, scene->bone);
    enemy.dest = {200, 620};
    enemy.emitter = {22, 360.0f/22.0f, 0, 110, 1.54f, twirl};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skelly, 1990, {480, 1080}, {150, -100}, 1550, scene->ball);
    enemy.emitter = {22, 360.0f/20.0f, 0, 135, 1.0f, spiral};
    level->enemies.push_back(enemy);
    enemy.emitter = {40, 360.0f/35.0f, 0, 185, 0.7f, twirl};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->skelly, 1990, {480, 1080}, {150, -100}, 1550, scene->bone);
    enemy.emitter = {14, 360.0f/14.0f, 0, 100, 1.7f, spread};
    level->enemies.push_back(enemy);
    enemy.emitter = {10, 360.0f/10.0f, 0, 80, 1.7f, twirl};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skull, 3150, {50, 680}, {70, 255}, 800, scene->bone);
    enemy.emitter = {4, 90, 0, 10, 3.0, expanding_shield};
    level->enemies.push_back(enemy);
    enemy.dest = {50, 1280};
    level->enemies.push_back(enemy);
    enemy.dest = {50, 1380};
    level->enemies.push_back(enemy);
    enemy.dest = {450, 1480};
    level->enemies.push_back(enemy);
    enemy.dest = {450, 1580};
    level->enemies.push_back(enemy);
    enemy.dest = {380, 1480};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 3200, {350, 1400}, {70, 299}, 800, scene->ball);
    enemy.emitter = {23, 360.0f/23.0f, 0, 66, 1.1f, spread};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->fairy, 4000, {250, 1500}, {70, 299}, 1400, scene->ball);
    enemy.emitter = {10, 360.0f/10.0f, 0, 75, 4.5f, spread};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->fairy, 4000, {150, 1300}, {70, 299}, 1400, scene->bigball);
    enemy.emitter = {10, 360.0f/10.0f, 0, 75, 4.5f, spread};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->fairy, 4000, {420, 1650}, {70, 299}, 1400, scene->ball);
    enemy.emitter = {10, 360.0f/10.0f, 0, 100, 4.5f, spiral};
    level->enemies.push_back(enemy);

    populate_enemy(&enemy, scene->skelly, 4200, {350, 1750}, {-500, 500}, 1200, scene->ball);
    enemy.emitter = {20, 360.0f/20.0f, 0, 90, 0.20f, expanding_shield};
    level->enemies.push_back(enemy);
    enemy.emitter = {20, 360.0f/20.0f, 0, 90, 0.20f, expanding_shield_reverse};
    level->enemies.push_back(enemy);
    populate_enemy(&enemy, scene->skull, 4200, {350, 1750}, {-500, 500}, 1600, scene->bone);
    enemy.emitter = {20, 360.0f/20.0f, 0, 190, 1.20f, spiral};
    level->enemies.push_back(enemy);
}

#endif
