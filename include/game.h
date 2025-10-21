#pragma once

#include <stdlib.h>

#include "config.h"
#include "event_system.h"
#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "audio.h"
#include "texture.h"
#include "game_state.h"

typedef struct {
    es_event_bus_t *bus;

    audio_t audio;
    texture_t texture;

    size_t map_idx;

    paddle_t paddle;
    ball_t ball;
    brick_t bricks[BRICKS_ROWS * BRICKS_COLS];

    game_state_t state;
} game_t;

game_t *game_create(void);
void game_destroy(game_t *game);

void game_input(game_t *game);
void game_update(game_t *game, float dt);
void game_render(const game_t *game);