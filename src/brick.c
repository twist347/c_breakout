#include "brick.h"

#include <assert.h>
#include <stddef.h>

#include "config.h"
#include "util.h"

static Color brick_type_to_color(brick_type_e type);

static int brick_type_to_lives(brick_type_e type);

static void handle_brick_event(const es_event_t *event, es_event_bus_t *bus, void *ctx);

void brick_init(brick_t *brick, Rectangle rect, bool active, brick_type_e type) {
    assert(brick);

    brick->rect = rect;
    brick->active = active;
    brick->type = type;
    brick->color = brick_type_to_color(brick->type);
    brick->lives = brick_type_to_lives(brick->type);
}

void brick_render(const brick_t *brick) {
    assert(brick);

    if (brick->active) {
        DrawRectangleRec(brick->rect, brick->color);
    }
}

void brick_render_grid(const brick_t *bricks) {
    assert(bricks);

    for (size_t i = 0; i < BRICKS_ROWS * BRICKS_COLS; ++i) {
        brick_render(&bricks[i]);
    }
}

int brick_type_to_score(brick_type_e type) {
    assert(type < BRICK_TYPE__COUNT);

    static const int brick_score[] = {
        [BRICK_TYPE_LIGHT] = BRICK_LIGHT_SCORE,
        [BRICK_TYPE_MEDIUM] = BRICK_MEDIUM_SCORE,
        [BRICK_TYPE_HEAVY] = BRICK_HEAVY_SCORE
    };
    return brick_score[type];
}

static Color brick_type_to_color(brick_type_e type) {
    assert(type < BRICK_TYPE__COUNT);

    static const Color brick_colors[] = {
        [BRICK_TYPE_LIGHT] = BRICK_LIGHT_COLOR,
        [BRICK_TYPE_MEDIUM] = BRICK_MEDIUM_COLOR,
        [BRICK_TYPE_HEAVY] = BRICK_HEAVY_COLOR,
    };
    return brick_colors[type];
}

static int brick_type_to_lives(brick_type_e type) {
    assert(type < BRICK_TYPE__COUNT);

    static const int brick_lives[] = {
        [BRICK_TYPE_LIGHT] = BRICK_LIGHT_LIVES,
        [BRICK_TYPE_MEDIUM] = BRICK_MEDIUM_LIVES,
        [BRICK_TYPE_HEAVY] = BRICK_HEAVY_LIVES
    };
    return brick_lives[type];
}

void brick_register_events(es_event_bus_t *bus, brick_t *bricks) {
    assert(bus);
    assert(bricks);

    es_subscribe(bus, EV_BRICK_LIFE_LOST, handle_brick_event, bricks);
}

static void handle_brick_event(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ARK_UNUSED(bus);

    ES_CTX_EXPECT(ctx, brick_t);
    brick_t *bricks = ES_CTX_PTR(ctx, brick_t);

    switch (es_get_event_type(event)) {
        case EV_BRICK_LIFE_LOST: {
            ES_EV_EXPECT(event, size_t);
            const size_t brick_idx = ES_EV_VAL(event, size_t);
            brick_t *brick = &bricks[brick_idx];
            --brick->lives;
            if (bricks[brick_idx].lives == 0) {
                bricks[brick_idx].active = false;
            }
            break;
        }
        default:
            break;
    }
}
