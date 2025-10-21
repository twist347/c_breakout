#include "menu.h"

#include <assert.h>

#include "raylib.h"

#include "config.h"

void menu_render(const texture_t *texture) {
    assert(texture);

    texture_render(texture, ASSET_TEXTURE_MENU);

    static const char *msg1 = "ARKANOID";
    static const char *msg2 = "PRESS [ENTER] TO START";
    static const char *msg3 = "PRESS [ESC] TO EXIT";
    const int w1 = MeasureText(msg1, MAIN_MENU_TITLE_FONT_SIZE);
    const int w2 = MeasureText(msg2, MAIN_MENU_FONT_SIZE);
    const int w3 = MeasureText(msg3, MAIN_MENU_FONT_SIZE);
    DrawText(
        msg1,
        (SCREEN_WIDTH - w1) / 2, SCREEN_HEIGHT / 2 - 100,
        MAIN_MENU_TITLE_FONT_SIZE,
        MAIN_MENU_TITLE_FONT_COLOR
    );
    DrawText(
        msg2,
        (SCREEN_WIDTH - w2) / 2, SCREEN_HEIGHT / 2 + 80,
        MAIN_MENU_FONT_SIZE,
        MAIN_MENU_FONT_COLOR
    );
    DrawText(
        msg3,
        (SCREEN_WIDTH - w3) / 2, SCREEN_HEIGHT / 2 + 160,
        MAIN_MENU_FONT_SIZE,
        MAIN_MENU_FONT_COLOR
    );
}

void menu_pause_render(const texture_t *texture) {
    assert(texture);

    texture_render(texture, ASSET_TEXTURE_MENU);

    static const char *msg1 = "PAUSED";
    static const char *msg2 = "PRESS [P] TO RESUME OR [ESC] TO EXIT GAME";
    const int w1 = MeasureText(msg1, MAIN_MENU_TITLE_FONT_SIZE);
    const int w2 = MeasureText(msg2, HUD_FONT_SIZE);
    DrawText(
        msg1,
        (SCREEN_WIDTH - w1) / 2, SCREEN_HEIGHT / 2 - 120,
        MAIN_MENU_TITLE_FONT_SIZE,
        MAIN_MENU_TITLE_FONT_COLOR
    );
    DrawText(msg2, (SCREEN_WIDTH - w2) / 2, SCREEN_HEIGHT / 2 + 40, HUD_FONT_SIZE, HUD_FONT_COLOR);
}

void menu_win_render(const texture_t *texture) {
    assert(texture);

    texture_render(texture, ASSET_TEXTURE_MENU);

    static const char *win = "YOU WIN!";
    static const char *rst = "PRESS [ENTER] TO RESTART ([ESC] TO EXIT GAME)";
    const int w1 = MeasureText(win, HUD_FONT_SIZE);
    const int w2 = MeasureText(rst, HUD_FONT_SIZE);
    DrawText(win, (SCREEN_WIDTH - w1) / 2, SCREEN_HEIGHT / 2 - 10, HUD_FONT_SIZE, HUD_FONT_COLOR);
    DrawText(rst, (SCREEN_WIDTH - w2) / 2, SCREEN_HEIGHT / 2 + 50, HUD_FONT_SIZE, HUD_FONT_COLOR);
}

void menu_lose_render(const texture_t *texture) {
    assert(texture);

    texture_render(texture, ASSET_TEXTURE_MENU);

    static const char *over = "GAME OVER";
    static const char *rst = "PRESS [ENTER] TO RESTART ([ESC] TO EXIT GAME)";
    const int w1 = MeasureText(over, HUD_FONT_SIZE);
    const int w2 = MeasureText(rst, HUD_FONT_SIZE);
    DrawText(over, (SCREEN_WIDTH - w1) / 2, SCREEN_HEIGHT / 2 - 10, HUD_FONT_SIZE, HUD_FONT_COLOR);
    DrawText(rst, (SCREEN_WIDTH - w2) / 2, SCREEN_HEIGHT / 2 + 50, HUD_FONT_SIZE, HUD_FONT_COLOR);
}
