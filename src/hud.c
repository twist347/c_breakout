#include "hud.h"

#include <assert.h>

#include "raylib.h"

#include "config.h"

static void render_lives(int lives);

static void render_score(int score);

void hud_render(int lives, int score) {
    render_lives(lives);
    render_score(score);
}

static void render_lives(int lives) {
    assert(lives >= 0);

    static const float padding = 10.f;
    for (int i = 0; i < lives; ++i) {
        const Rectangle rect = {
            .x = 20.f + (float) i * (HUD_LIVE_RECT.x + padding),
            .y = SCREEN_HEIGHT - 50.f,
            .width = HUD_LIVE_RECT.x,
            .height = HUD_LIVE_RECT.y
        };
        DrawRectangleRec(rect, HUD_LIVE_RECT_COLOR);
    }
}

static void render_score(int score) {
    assert(score >= 0);

    const char *score_msg = TextFormat("score: %d", score);
    DrawText(
        score_msg,
        SCREEN_WIDTH - 24 - MeasureText(score_msg, HUD_FONT_SIZE), SCREEN_HEIGHT - 75,
        HUD_FONT_SIZE,
        HUD_FONT_COLOR
    );
}

