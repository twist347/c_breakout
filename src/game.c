#include "game.h"

#include <assert.h>

#include "hud.h"
#include "map.h"
#include "menu.h"

static void game_register_events(es_event_bus_t *bus, game_t *game);

static void register_subscribers(game_t *game);

static void handle_key_pressed(const es_event_t *event, es_event_bus_t *bus, void *ctx);

static void handle_key_down(const es_event_t *event, es_event_bus_t *bus, void *ctx);

static void handle_game_event(const es_event_t *event, es_event_bus_t *bus, void *ctx);

static void game_load_map(game_t *game, size_t map_idx);

static void render_gameplay(const game_t *game);

game_t *game_create(void) {
    game_t *game = malloc(sizeof(game_t));
    if (!game) {
        TraceLog(LOG_ERROR, "failed to allocate memory for game_t");
        return NULL;
    }

    es_event_bus_t *bus = es_bus_create();
    if (!bus) {
        TraceLog(LOG_ERROR, "failed to allocate memory for event bus");
        free(game);
        return NULL;
    }

    game->bus = bus;

    register_subscribers(game);

    if (!audio_init(&game->audio)) {
        TraceLog(LOG_ERROR, "failed to initialize audio");
    }

    if (!texture_init(&game->texture)) {
        TraceLog(LOG_ERROR, "failed to initialize texture");
    }

    es_publish(game->bus, ES_EV_GAME_START);
    game_screen_change(&game->state.screen, GAME_SCREEN_MENU);

    return game;
}

void game_destroy(game_t *game) {
    assert(game);

    texture_unload(&game->texture);
    audio_unload(&game->audio);
    es_bus_destroy(game->bus);

    free(game);
}

void game_input(game_t *game) {
    assert(game);

    for (int key = GetKeyPressed(); key != 0; key = GetKeyPressed()) {
        ES_PUBLISH_LDATA(game->bus, ES_EV_KEY_PRESSED, key);
    }

    if (game->state.screen == GAME_SCREEN_PLAYING) {
        const int move = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        ES_PUBLISH_LDATA(game->bus, ES_EV_KEY_DOWN, move);
    }
}

void game_update(game_t *game, float dt) {
    assert(game);

    audio_update(&game->audio);

    switch (game->state.screen) {
        case GAME_SCREEN_MENU:
            break;
        case GAME_SCREEN_PLAYING: {
            paddle_t *paddle = &game->paddle;
            ball_t *ball = &game->ball;
            const Vector2 screen_res = {SCREEN_WIDTH, SCREEN_HEIGHT};

            paddle_update(paddle, dt);
            if (game->ball.launched) {
                ball_update(ball, dt);
            } else {
                ball_follow_paddle(ball, &paddle->rect);
            }

            if (ball_check_bottom_collision(ball, screen_res)) {
                es_publish(game->bus, ES_EV_GAME_LIFE_LOST);
                break;
            }

            if (ball_check_left_wall_collision(ball, screen_res)) {
                ES_PUBLISH_RDATA(game->bus, ES_EV_BALL_HIT_WALL, Vector2, ((Vector2){1.f, 0.f}));
                es_publish(game->bus, ES_EV_SFX_PLAY_WALL_HIT);
            } else if (ball_check_right_wall_collision(ball, screen_res)) {
                ES_PUBLISH_RDATA(game->bus, ES_EV_BALL_HIT_WALL, Vector2, ((Vector2){-1.f, 0.f}));
                es_publish(game->bus, ES_EV_SFX_PLAY_WALL_HIT);
            } else if (ball_check_top_wall_collision(ball, screen_res)) {
                ES_PUBLISH_RDATA(game->bus, ES_EV_BALL_HIT_WALL, Vector2, ((Vector2){0.f, 1.f}));
                es_publish(game->bus, ES_EV_SFX_PLAY_WALL_HIT);
            }

            if (ball_check_paddle_collision(ball, &paddle->rect)) {
                ES_PUBLISH_LDATA(game->bus, ES_EV_BALL_HIT_PADDLE, paddle->rect);
                es_publish(game->bus, ES_EV_SFX_PLAY_BOUNCE);
                break;
            }
            for (size_t i = 0; i < BRICKS_ROWS * BRICKS_COLS; ++i) {
                const brick_t *brick = &game->bricks[i];
                if (brick->active && ball_check_rect_collision(ball, &brick->rect)) {
                    es_publish(game->bus, ES_EV_SFX_PLAY_WALL_HIT);
                    ES_PUBLISH_LDATA(game->bus, ES_EV_BALL_HIT_BRICK, brick->rect);
                    ES_PUBLISH_LDATA(game->bus, ES_EV_BRICK_LIFE_LOST, i);
                    if (brick->lives == 0) {
                        es_publish(game->bus, ES_EV_GAME_DECREASE_BRICKS_COUNT);
                        const int score = brick_type_to_score(brick->type);
                        ES_PUBLISH_LDATA(game->bus, ES_EV_GAME_INCREASE_SCORE, score);
                    }

                    break;
                }
            }
            break;
        }
        case GAME_SCREEN_PAUSE:
        case GAME_SCREEN_WIN:
        case GAME_SCREEN_LOSE:
        default:
            break;
    }
}

void game_render(const game_t *game) {
    assert(game);

    BeginDrawing();
    ClearBackground(BG_COLOR);

    switch (game->state.screen) {
        case GAME_SCREEN_MENU:
            menu_render(&game->texture);
            break;
        case GAME_SCREEN_PLAYING:
            render_gameplay(game);
            break;
        case GAME_SCREEN_PAUSE:
            menu_pause_render(&game->texture);
            break;
        case GAME_SCREEN_WIN:
            menu_win_render(&game->texture);
            break;
        case GAME_SCREEN_LOSE:
            menu_lose_render(&game->texture);
            break;
        default:
            break;
    }

    DrawFPS(10, 10);

    EndDrawing();
}

static void game_register_events(es_event_bus_t *bus, game_t *game) {
    assert(bus);
    assert(game);

    assert(es_subscribe(bus, ES_EV_GAME_START, handle_game_event, game));
    assert(es_subscribe(bus, ES_EV_KEY_PRESSED, handle_key_pressed, game));
    assert(es_subscribe(bus, ES_EV_KEY_DOWN, handle_key_down, game));
}

static void register_subscribers(game_t *game) {
    assert(game);

    game_register_events(game->bus, game);
    game_state_register_events(game->bus, &game->state);
    paddle_register_events(game->bus, &game->paddle);
    ball_register_events(game->bus, &game->ball);
    brick_register_events(game->bus, game->bricks);
    audio_register_events(game->bus, &game->audio);
}

static void handle_key_pressed(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ES_CTX_EXPECT(ctx, game_t);
    const game_t *game = ES_CTX_CPTR(ctx, game_t);

    ES_EV_EXPECT(event, int);
    const int key = ES_EV_VAL(event, int);

    switch (game->state.screen) {
        case GAME_SCREEN_MENU:
            if (key == KEY_ENTER) {
                es_publish(bus, ES_EV_GAME_START);
            }
            break;
        case GAME_SCREEN_PLAYING:
            if (key == KEY_P) {
                es_publish(bus, ES_EV_GAME_PAUSE);
            }
            if (key == KEY_SPACE && !game->ball.launched) {
                es_publish(bus, ES_EV_BALL_LAUNCHED);
            }
            break;
        case GAME_SCREEN_PAUSE:
            if (key == KEY_P) {
                es_publish(bus, ES_EV_GAME_RESUME);
            }
            break;
        case GAME_SCREEN_WIN:
        case GAME_SCREEN_LOSE:
            if (key == KEY_ENTER) {
                es_publish(bus, ES_EV_GAME_START);
            }
            break;
        default:
            break;
    }
}

static void handle_key_down(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ES_CTX_EXPECT(ctx, game_t);
    const game_t *game = ES_CTX_CPTR(ctx, game_t);

    if (game->state.screen == GAME_SCREEN_PLAYING) {
        ES_EV_EXPECT(event, int);
        const int move = ES_EV_VAL(event, int);
        switch (move) {
            case 0:
                es_publish(bus, ES_EV_PADDLE_MOVE_NONE);
                break;
            case 1:
                es_publish(bus, ES_EV_PADDLE_MOVE_RIGHT);
                break;
            case -1:
                es_publish(bus, ES_EV_PADDLE_MOVE_LEFT);
                break;
            default:
                break;
        }
    }
}

static void handle_game_event(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ES_CTX_EXPECT(ctx, game_t);
    game_t *game = ES_CTX_PTR(ctx, game_t);

    switch (es_get_event_type(event)) {
        case ES_EV_GAME_START:
            game->map_idx = MAP_INIT_IDX;

            paddle_init(&game->paddle, PADDLE_INIT_RECT, PADDLE_INIT_VEL, PADDLE_COLOR);
            ball_init(&game->ball, BALL_INIT_POS, BALL_RADIUS, BALL_INIT_VEL, BALL_COLOR);

            game_state_init(&game->state, GAME_SCREEN_PLAYING, 0, 3, 0);

            game_load_map(game, game->map_idx);
            break;
        default:
            break;
    }
}

static void game_load_map(game_t *game, size_t map_idx) {
    assert(game);

    game->state.n_bricks_alive = 0;

    const char (*map)[BRICKS_COLS] = get_map(map_idx);

    for (size_t i = 0; i < BRICKS_ROWS; ++i) {
        for (size_t j = 0; j < BRICKS_COLS; ++j) {
            brick_t *brick = &game->bricks[i * BRICKS_COLS + j];
            brick_type_e type = BRICK_TYPE_LIGHT;
            bool active = false;
            const char symbol = map[i][j];
            switch (symbol) {
                case BRICK_SYMBOL_LIGHT:
                    type = BRICK_TYPE_LIGHT;
                    active = true;
                    break;
                case BRICK_SYMBOL_MEDIUM:
                    type = BRICK_TYPE_MEDIUM;
                    active = true;
                    break;
                case BRICK_SYMBOL_HEAVY:
                    type = BRICK_TYPE_HEAVY;
                    active = true;
                    break;
                case BRICK_SYMBOL_EMPTY:
                default:
                    active = false;
            }

            if (active) {
                ++game->state.n_bricks_alive;
            }

            brick_init(
                brick,
                (Rectangle){
                    .x = BRICK_GRID_X + (BRICK_WIDTH + BRICK_PADDING_X) * (float) j,
                    .y = BRICK_GRID_Y + (BRICK_HEIGHT + BRICK_PADDING_Y) * (float) i,
                    .width = BRICK_WIDTH, .height = BRICK_HEIGHT
                },
                active,
                type
            );
        }
    }
}

static void render_gameplay(const game_t *game) {
    assert(game);

    texture_render(&game->texture, ASSET_TEXTURE_BG);
    brick_render_grid(game->bricks);
    paddle_render(&game->paddle);
    ball_render(&game->ball);
    hud_render(game->state.lives, game->state.score);

    if (!game->ball.launched) {
        static const char *msg = "PRESS [SPACE] TO LAUNCH";
        const int w = MeasureText(msg, HUD_FONT_SIZE);
        DrawText(
            msg,
            (SCREEN_WIDTH - w) / 2, SCREEN_HEIGHT / 2 + 200,
            HUD_FONT_SIZE,
            HUD_FONT_COLOR
        );
    }
}
