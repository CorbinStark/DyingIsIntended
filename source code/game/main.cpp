#include "bahamut.h"
#include "utils.h"
#include "map.h"
#include "level.h"
#include "stages.h"

enum MainState {
    GOTO_TITLE,
    MAIN_TITLE,

    GOTO_GAME,
    MAIN_GAME,

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
    if(text_button(batch, font, " OPTIONS", &yInitial, mouse)) {

    }
    if(text_button(batch, font, " CREDITS", &yInitial, mouse)) {

    }
    if(text_button(batch, font, " QUIT", &yInitial, mouse)) {
        *mainstate = MAIN_EXIT;
    }
}

int main() {
    printf("\n/////////////////////////////////\nPROGRAM STARTING\n/////////////////////////////////\n\n");
    init_window(640, 360, "Dying Is Intended", false, true, true);
    init_audio();
    set_fps_cap(60);
    set_master_volume(100);
    set_vsync(true);
    set_mouse_state(MOUSE_HIDDEN);

    RenderBatch * batch = &create_batch();
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

    levels[9].map=load_map("data/map1.txt");
    levels[9].scroll = SCROLL_RIGHT;
    levels[9].map.y = -30;
    levels[9].init = test_stage;

    i32 currlevel = 0;

    Editor edit = {0};
    edit.map = create_map(44, 125);

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
            if(state == MAIN_GAME) {
                level(batch, &levels[currlevel], &levelscene, &currlevel);
                //editor(batch, &edit, &levelscene, mouse);
            }
            if(state == GOTO_GAME) {
                state = MAIN_GAME;
            }
            if(state == GOTO_TITLE) {
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
