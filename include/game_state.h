#pragma once

#include "game_screen.h"

#include "event_system.h"

typedef struct {
    game_screen_e screen;

    int n_bricks_alive;
    int lives;
    int score;
} game_state_t;

void game_state_init(game_state_t* state, game_screen_e screen, int n_bricks_alive, int lives, int score);
void game_state_register_events(es_event_bus_t *bus, game_state_t *game_state);