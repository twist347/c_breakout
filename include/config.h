#pragma once

#include "raylib.h"

#define ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

// screen config
#define SCREEN_WIDTH     1920
#define SCREEN_HEIGHT    1080
#define SCREEN_TITLE     "c_breakout"
#define TARGET_FPS       60
#define BG_COLOR         BLACK

// paddle config
#define PADDLE_WIDTH        250
#define PADDLE_HEIGHT       30
#define PADDLE_INIT_VEL     700.f
#define PADDLE_COLOR        BLUE
#define PADDLE_INIT_RECT    (Rectangle){SCREEN_WIDTH * 0.5f - PADDLE_WIDTH * 0.5f, SCREEN_HEIGHT - 75.f, PADDLE_WIDTH, PADDLE_HEIGHT}

// ball config
#define BALL_RADIUS      20.f
#define BALL_INIT_VEL    ((Vector2){500.f, 500.f})
#define BALL_COLOR       RED
#define BALL_INIT_POS    (Vector2){SCREEN_WIDTH * 0.5f, PADDLE_INIT_RECT.y - BALL_RADIUS - 5.f}

// brick config
#define BRICKS_ROWS           4
#define BRICKS_COLS           7
#define BRICK_WIDTH           160.f
#define BRICK_HEIGHT          30.f
#define BRICK_PADDING_X       20.f
#define BRICK_PADDING_Y       50.f
#define BRICK_GRID_WIDTH      (BRICKS_COLS * BRICK_WIDTH + (BRICKS_COLS - 1) * BRICK_PADDING_X)
#define BRICK_GRID_X          (SCREEN_WIDTH * 0.5f - BRICK_GRID_WIDTH * 0.5f)
#define BRICK_GRID_Y          50.f

#define BRICK_LIGHT_COLOR     YELLOW
#define BRICK_MEDIUM_COLOR    PINK
#define BRICK_HEAVY_COLOR     BROWN

#define BRICK_LIGHT_LIVES     1
#define BRICK_MEDIUM_LIVES    2
#define BRICK_HEAVY_LIVES     3

#define BRICK_LIGHT_SCORE     100
#define BRICK_MEDIUM_SCORE    200
#define BRICK_HEAVY_SCORE     300

// Main menu config
#define MAIN_MENU_TITLE_FONT_COLOR    RED
#define MAIN_MENU_TITLE_FONT_SIZE     120
#define MAIN_MENU_FONT_COLOR          WHITE
#define MAIN_MENU_FONT_SIZE           70

// HUD config
#define HUD_FONT_SIZE     50
#define HUD_FONT_COLOR    WHITE

#define HUD_LIVE_RECT_COLOR    GREEN
#define HUD_LIVE_RECT          (Vector2){40.f, 20.f}

// map config
#define MAP_INIT_IDX    0

// sfx config
#define ASSET_SFX_BOUNCE_PATH      "/sound/bounce.wav"
#define ASSET_SFX_WALL_HIT_PATH    "/sound/hit.wav"

// music config
#define ASSET_MUSIC_BG_PATH        "/music/bg_music.ogg"

// textures config
#define ASSET_TEXTURE_BG_STARFIELD_PATH    "/textures/bg_starfield.png"
#define ASSET_TEXTURE_MENU_AURORA_PATH     "/textures/menu_aurora.png"