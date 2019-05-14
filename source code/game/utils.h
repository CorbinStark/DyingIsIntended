#ifndef UTILS_H
#define UTILS_H

#include "bahamut.h"
#include <random>

struct Animation {
    Texture img;
    u16 frames;
    u16 current;
    u16 y;
    u16 width;
    u16 height;
    u32 delay;
    f32 scale;
};

static inline
Animation create_animation(Texture sheet, u16 frames, u16 y, u16 width, u16 height, u32 delay, f32 scale = 1) {
    Animation anim;
    anim.current = 0;
    anim.y = y;
    anim.img = sheet;
    anim.frames = frames;
    anim.width = width;
    anim.height = height;
    anim.delay = delay;
    anim.scale = scale;
    return anim;
}

static inline
void update_animation(Animation* anim, i32 timer) {
    if (timer % anim->delay == 0)
        anim->current++;
    if (anim->current >= anim->frames)
        anim->current = 0;
}

static inline
void draw_animation(RenderBatch* batch, Animation anim, f32 x, f32 y, bool right = true) {
    Rect src = { anim.current * anim.width, anim.y, anim.width, anim.height };
    Rect dst = { x, y, anim.width * anim.scale, anim.height * anim.scale };
    if(!right) anim.img.flip_flag = FLIP_HORIZONTAL;
    draw_texture_EX(batch, anim.img, src, dst);
}

static inline
i32 random_int(i32 min, i32 max) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, max-min);
    return dist(mt) + min;
}

struct BitmapFont {
    Texture chars[SCHAR_MAX];
};

static inline Texture get_sub_image(unsigned char* pixels, i32 pixels_width, i32 x, i32 y, i32 width, i32 height, i32 texparam) {
    Texture subimage;
    glPixelStorei(GL_UNPACK_ROW_LENGTH, pixels_width);
    unsigned char* subimage_pixels = pixels + (x + y * pixels_width) * 4;
    subimage = load_texture(subimage_pixels, width, height, texparam);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    return subimage;
}

static inline
BitmapFont load_1bit_font() {
    BitmapFont font = {0};
    i32 w;
    i32 h;
    unsigned char* image = SOIL_load_image("data/art/font.png", &w, &h, 0, SOIL_LOAD_RGBA);
    i32 x = 0;
    for(u16 i = 0; i < 13; ++i) {
        font.chars[i+65] = get_sub_image(image, w, x, 16, 16, 16, GL_NEAREST);
        x += 17;
    }
    x = 0;
    for(u16 i = 0; i < 13; ++i) {
        font.chars[i+65+13] = get_sub_image(image, w, x, 33, 16, 16, GL_NEAREST);
        x += 17;
    }
    free(image);
    return font;
}

static inline
void draw_text(RenderBatch* batch, BitmapFont* font, const char* str, f32 x, f32 y) {
    i32 len = strlen(str);
    for(u16 i = 0; i < len; ++i) {
        draw_texture(batch, font->chars[str[i]], x, y);
        x += 16;
    }
}

static inline
i32 get_string_width(const char* str) {
    return strlen(str) * 16;
}

static inline
f32 get_angle(vec2 a, vec2 b) {
    return atan2(b.y - a.y, b.x - a.x) * 180 / 3.14159;
}

#endif
