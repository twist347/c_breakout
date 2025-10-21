#pragma once

typedef enum {
    GAME_SCREEN_MENU = 0,
    GAME_SCREEN_PLAYING,
    GAME_SCREEN_PAUSE,
    GAME_SCREEN_WIN,
    GAME_SCREEN_LOSE,
    GAME_SCREEN__COUNT
} game_screen_e;

void game_screen_change(game_screen_e *from, game_screen_e to);
