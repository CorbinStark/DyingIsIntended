#include "bahamut.h"
#include "utils.h"
#include "map.h"
#include "level.h"
#include "stages.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

enum MainState {
    GOTO_TITLE,
    MAIN_TITLE,

    GOTO_GAME,
    MAIN_GAME,

    MAIN_CONTROLS,
    MAIN_CREDITS,

    MAIN_EXIT
};

static inline
bool text_button(RenderBatch* batch, BitmapFont* font, std::string str, i32* yInitial, vec2 mouse) {
    f32 height = 16 * 2;
    f32 width = get_string_width(str.c_str());
    f32 xPos = (get_virtual_width() / 2) - (width / 2);
    f32 yPos = *yInitial += (16*2);
    bool collided = colliding({ xPos, yPos, width, height }, { mouse.x, mouse.y });
    if (collided) {
        str[0] = 'A'-3;
        draw_text(batch, font, str.c_str(), xPos+5, yPos);
    }
    else {
        str[0] = ' ';
        draw_text(batch, font, str.c_str(), xPos, yPos);
    }

    return collided & is_button_released(MOUSE_BUTTON_LEFT);
}

static inline
void title_screen(RenderBatch* batch, BitmapFont* font, MainState* mainstate, vec2 mouse) {
    draw_text(batch, font, "DYING IS INTENDED", (get_virtual_width()/2) - (get_string_width("Dying Is Intended")/2), 10);

    i32 yInitial = 160;

    if(text_button(batch, font, " PLAY", &yInitial, mouse)) {
        *mainstate = GOTO_GAME;
    }
    if(text_button(batch, font, " CONTROLS", &yInitial, mouse)) {
        *mainstate = MAIN_CONTROLS;
    }
    if(text_button(batch, font, " CREDITS", &yInitial, mouse)) {
        *mainstate = MAIN_CREDITS;
    }
    if(text_button(batch, font, " QUIT", &yInitial, mouse)) {
        *mainstate = MAIN_EXIT;
    }
}

int main() {
    printf("\n/////////////////////////////////\n        PROGRAM STARTING\n/////////////////////////////////\n\n");
    init_window(640, 360, "Dying Is Intended", false, true, true);
    init_audio();
    set_fps_cap(60);
    set_master_volume(100);
    set_vsync(true);
    set_mouse_state(MOUSE_HIDDEN);

    RenderBatch* batch = &create_batch();
    Shader basic = load_default_shader_2D();
    MainState state = GOTO_TITLE;

    Texture cursor = load_texture("data/art/cursor.png", GL_NEAREST);
    BitmapFont font = load_1bit_font();

    start_shader(basic);
    upload_mat4(basic, "projection", orthographic_projection(0, 0, get_window_width(), get_window_height(), -1, 1));
    stop_shader();

    LevelScene levelscene = {0};
    levelscene = load_level_scene();
    Level levels[10] = {0};
    levels[0].map = load_map("data/map2.txt");
    levels[0].init = stage0;
    levels[0].scroll = SCROLL_NONE;
    
    levels[1].map = load_map("data/map1.txt");
    levels[1].init = stage1;
    levels[1].scroll = SCROLL_RIGHT;

    levels[2].map = load_map("data/map3.txt");
    levels[2].scroll = SCROLL_RIGHT;
    levels[2].map.y = -30;
    levels[2].init = stage2;

    levels[3].map = load_map("data/map4.txt");
    levels[3].scroll = SCROLL_DOWN;
    levels[3].map.y = -1000;
    levels[3].init = stage3;

    levels[4].map = load_map("data/map5.txt");
    levels[4].scroll = SCROLL_NONE;
    levels[4].map.x = -175;
    levels[4].map.y = 0;
    levels[4].init = stage4;

    levels[9].map=load_map("data/map1.txt");
    levels[9].scroll = SCROLL_RIGHT;
    levels[9].map.y = -30;
    levels[9].init = test_stage;

    i32 currlevel = 0;

    Editor edit = {0};
    edit.map = create_map(44, 25);

    Sound menu = load_sound("data/sound/Memoraphile - Spooky Dungeon.wav");
    Sound battle = load_sound("data/sound/8BitBattleLoop.wav");
    set_sound_looping(menu, true);
    set_sound_looping(battle, true);

    while(window_open()) {
        Rect view = fit_aspect_ratio(1.777777777777778);
        vec2 mouse = get_mouse_pos(view);
        set_viewport(view.x, view.y, view.width, view.height);

        begin_drawing();
        begin2D(batch, basic);
        draw_rectangle(batch, 0, 0, 640, 360, {71, 45, 60, 255});
            
            if(state == MAIN_TITLE) {
                title_screen(batch, &font, &state, mouse);
            }
            if(state == MAIN_CONTROLS) {
                i32 starty = 130;
                draw_text(batch, &font, "Z OR SPACE - JUMP", 10, starty + (17*0));
                draw_text(batch, &font, "C          - BOMB", 10, starty + (17*1));
                draw_text(batch, &font, "ARROW KEYS - MOVE", 10, starty + (17*2));
                draw_text(batch, &font, "DOWN ARROW - FALL THROUGH PLATFORM", 10, starty + (17*3));
                draw_text(batch, &font, "SHIFT      - SLOW DOWN", 10, starty + (17*4));
                draw_text(batch, &font, "P OR ESCAPE  PAUSE", 10, starty + (17*5));
                draw_text(batch, &font, "ANY KEY      EXIT THIS MENU", 10, starty + (17*6));
                if(get_key_pressed() != 0) state = MAIN_TITLE;
            }
            if(state == MAIN_CREDITS) {
                draw_text(batch, &font, "FOLLOWING WORKS WERE USED", 10, 10);
                draw_text(batch, &font, "ONE-BIT PACK    (MODIFIED)", 10, 70);
                draw_text(batch, &font, "EIGHT-BIT BATTLE LOOP", 10, 70+30);
                draw_text(batch, &font, "SPOOKY DUNGEON", 10, 40+90);
                if(get_key_pressed() != 0) state = MAIN_TITLE;
            }
            if(state == MAIN_GAME) {
                if(currlevel == 5) {
                    draw_text(batch, &font, "YOU HAVE ESCAPED HELL.", 100, 100);
                    draw_text(batch, &font, "CONGRATULATIONS, MORTAL FAIRY.", 120, 200);
                } else
                    level(batch, &levels[currlevel], &levelscene, &currlevel);
                //editor(batch, &edit, &levelscene, mouse);
            }
            if(state == GOTO_GAME) {
                play_sound(battle);
                stop_sound(menu);
                state = MAIN_GAME;
            }
            if(state == GOTO_TITLE) {
                stop_sound(battle);
                play_sound(menu);
                state = MAIN_TITLE;
            }
            if(state == MAIN_EXIT) 
                exit(0);
            
            draw_texture(batch, cursor, mouse.x, mouse.y);
            
        end2D(batch);
        end_drawing();
    }

    dispose_batch(batch);
    dispose_window();

    return 0;
}
