#include "game_state.h"

#include <assert.h>

static void handle_game_state_event(const es_event_t *event, es_event_bus_t *bus, void *ctx);

void game_state_init(game_state_t* state, game_screen_e screen, int n_bricks_alive, int lives, int score) {
    assert(state);

    state->screen = screen;
    state->n_bricks_alive = n_bricks_alive;
    state->lives = lives;
    state->score = score;
}

void game_state_register_events(es_event_bus_t *bus, game_state_t *game_state) {
    assert(bus);
    assert(game_state);

    assert(es_subscribe(bus, EV_GAME_PAUSE, handle_game_state_event, game_state));
    assert(es_subscribe(bus, EV_GAME_RESUME, handle_game_state_event, game_state));
    assert(es_subscribe(bus, EV_GAME_WIN, handle_game_state_event, game_state));
    assert(es_subscribe(bus, EV_GAME_OVER, handle_game_state_event, game_state));
    assert(es_subscribe(bus, EV_GAME_LIFE_LOST, handle_game_state_event, game_state));
    assert(es_subscribe(bus, EV_GAME_DECREASE_BRICKS_COUNT, handle_game_state_event, game_state));
    assert(es_subscribe(bus, EV_GAME_INCREASE_SCORE, handle_game_state_event, game_state));
}

static void handle_game_state_event(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ES_CTX_EXPECT(ctx, game_state_t);
    game_state_t *state = ES_CTX_PTR(ctx, game_state_t);

    switch (es_get_event_type(event)) {
        case EV_GAME_PAUSE:
            game_screen_change(&state->screen, GAME_SCREEN_PAUSE);
            break;
        case EV_GAME_RESUME:
            game_screen_change(&state->screen, GAME_SCREEN_PLAYING);
            break;
        case EV_GAME_WIN:
            game_screen_change(&state->screen, GAME_SCREEN_WIN);
            break;
        case EV_GAME_OVER:
            game_screen_change(&state->screen, GAME_SCREEN_LOSE);
            break;
        case EV_GAME_LIFE_LOST:
            if (state->lives > 1) {
                --state->lives;
                es_publish(bus, EV_BALL_RESET);
                es_publish(bus, EV_PADDLE_RESET);
            } else {
                es_publish(bus, EV_GAME_OVER);
            }
            break;
        case EV_GAME_DECREASE_BRICKS_COUNT:
            --state->n_bricks_alive;
            if (state->n_bricks_alive == 0) {
                es_publish(bus, EV_GAME_WIN);
            }
            break;
        case EV_GAME_INCREASE_SCORE: {
            ES_EV_EXPECT(event, int);
            const int score = ES_EV_VAL(event, int);
            state->score += score;
            break;
        }
        default:
            break;
    }
}