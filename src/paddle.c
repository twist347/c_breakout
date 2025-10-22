#include "paddle.h"

#include <assert.h>

#include "raymath.h"

#include "config.h"
#include "util.h"

static void handle_paddle_event(const es_event_t *event, es_event_bus_t *bus, void *ctx);

void paddle_init(paddle_t *paddle, Rectangle rect, float vel, Color color) {
    assert(paddle);

    paddle->rect = rect;
    paddle->vel = vel;
    paddle->color = color;
    paddle->axis = 0;
}

void paddle_update(paddle_t *paddle, float dt) {
    assert(paddle);

    paddle->rect.x += paddle->vel * dt * (float) paddle->axis;
    paddle->rect.x = Clamp(paddle->rect.x, 0.f, (float) SCREEN_WIDTH - paddle->rect.width);
}

void paddle_render(const paddle_t *paddle) {
    assert(paddle);

    DrawRectangleRounded(paddle->rect, 1.f, 1, paddle->color);
}

void paddle_register_events(es_event_bus_t *bus, paddle_t *paddle) {
    assert(bus);
    assert(paddle);

    assert(es_subscribe(bus, ES_EV_PADDLE_MOVE_NONE, handle_paddle_event, paddle));
    assert(es_subscribe(bus, ES_EV_PADDLE_MOVE_LEFT, handle_paddle_event, paddle));
    assert(es_subscribe(bus, ES_EV_PADDLE_MOVE_RIGHT, handle_paddle_event, paddle));
    assert(es_subscribe(bus, ES_EV_PADDLE_RESET, handle_paddle_event, paddle));
}

static void handle_paddle_event(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ARK_UNUSED(bus);

    ES_CTX_EXPECT(ctx, paddle_t);
    paddle_t *paddle = ES_CTX_PTR(ctx, paddle_t);

    switch (es_get_event_type(event)) {
        case ES_EV_PADDLE_MOVE_NONE:
            paddle->axis = 0;
            break;
        case ES_EV_PADDLE_MOVE_LEFT:
            paddle->axis = -1;
            break;
        case ES_EV_PADDLE_MOVE_RIGHT:
            paddle->axis = 1;
            break;
        case ES_EV_PADDLE_RESET:
            paddle_init(paddle, PADDLE_INIT_RECT, PADDLE_INIT_VEL, PADDLE_COLOR);
            break;
        default:
            break;
    }
}