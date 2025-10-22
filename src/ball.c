#include "ball.h"

#include <assert.h>
#include <math.h>

#include "raymath.h"

#include "config.h"
#include "util.h"

static void handle_ball_event(const es_event_t *event, es_event_bus_t *bus, void *ctx);

void ball_init(ball_t *ball, Vector2 pos, float radius, Vector2 vel, Color color) {
    assert(ball);

    ball->pos = pos;
    ball->radius = radius;
    ball->vel = vel;
    ball->color = color;
    ball->launched = false;
}

void ball_update(ball_t *ball, float dt) {
    assert(ball);

    ball->pos.x += ball->vel.x * dt;
    ball->pos.y += ball->vel.y * dt;
}

void ball_follow_paddle(ball_t *ball, const Rectangle *paddle_rect) {
    assert(ball);
    assert(paddle_rect);

    ball->pos.x = paddle_rect->x + paddle_rect->width * 0.5f;
}

bool ball_check_rect_collision(const ball_t *ball, const Rectangle *rect) {
    assert(ball);
    assert(rect);

    return CheckCollisionCircleRec(ball->pos, ball->radius, *rect);
}

void ball_handle_rect_collision(ball_t *ball, const Rectangle *rect) {
    assert(ball);
    assert(rect);

    const float nearest_x = Clamp(ball->pos.x, rect->x, rect->x + rect->width);
    const float nearest_y = Clamp(ball->pos.y, rect->y, rect->y + rect->height);
    const float dx = ball->pos.x - nearest_x;
    const float dy = ball->pos.y - nearest_y;

    if (fabsf(dx) > fabsf(dy)) {
        ball->vel.x = -ball->vel.x;
        ball->pos.x = dx > 0.f ? rect->x + rect->width + ball->radius : rect->x - ball->radius;
    } else {
        ball->vel.y = -ball->vel.y;
        ball->pos.y = dy > 0.f ? rect->y + rect->height + ball->radius : rect->y - ball->radius;
    }
}

void ball_render(const ball_t *ball) {
    assert(ball);

    DrawCircleV(ball->pos, ball->radius, ball->color);
}

bool ball_check_bottom_collision(const ball_t *ball, Vector2 screen_res) {
    assert(ball);

    return ball->pos.y - ball->radius > screen_res.y;
}

bool ball_check_left_wall_collision(const ball_t *ball, Vector2 screen_res) {
    assert(ball);

    (void) screen_res;
    return ball->pos.x - ball->radius <= 0.f;
}

bool ball_check_right_wall_collision(const ball_t *ball, Vector2 screen_res) {
    assert(ball);

    return ball->pos.x + ball->radius >= screen_res.x;
}

bool ball_check_top_wall_collision(const ball_t *ball, Vector2 screen_res) {
    assert(ball);

    (void) screen_res;
    return ball->pos.y - ball->radius <= 0.f;
}

void ball_handle_walls_collision(ball_t *ball, Vector2 normal) {
    assert(ball);

    const float dot = ball->vel.x * normal.x + ball->vel.y * normal.y;
    ball->vel.x -= 2.f * dot * normal.x;
    ball->vel.y -= 2.f * dot * normal.y;
}

bool ball_check_paddle_collision(const ball_t *ball, const Rectangle *paddle_rect) {
    assert(ball);
    assert(paddle_rect);

    return CheckCollisionCircleRec(ball->pos, ball->radius, *paddle_rect);
}

void ball_handle_paddle_collision(ball_t *ball, const Rectangle *paddle_rect) {
    assert(ball);
    assert(paddle_rect);

    static const float BALL_MAX_BOUNCE_ANGLE = 75.f;

    float hit_position =
            (ball->pos.x - (paddle_rect->x + paddle_rect->width * 0.5f)) / (paddle_rect->width * 0.5f);

    hit_position = Clamp(hit_position, -1.f, 1.f);
    const float angle = hit_position * BALL_MAX_BOUNCE_ANGLE * DEG2RAD;
    const float speed = Vector2Length(ball->vel);

    ball->vel.x = sinf(angle) * speed;
    ball->vel.y = -fabsf(cosf(angle) * speed);

    ball->pos.y = paddle_rect->y - ball->radius - 1.f;
}

void ball_register_events(es_event_bus_t *bus, ball_t *ball) {
    assert(bus);
    assert(ball);

    assert(es_subscribe(bus, ES_EV_BALL_LAUNCHED, handle_ball_event, ball));
    assert(es_subscribe(bus, ES_EV_BALL_HIT_WALL, handle_ball_event, ball));
    assert(es_subscribe(bus, ES_EV_BALL_HIT_PADDLE, handle_ball_event, ball));
    assert(es_subscribe(bus, ES_EV_BALL_HIT_BRICK, handle_ball_event, ball));
    assert(es_subscribe(bus, ES_EV_BALL_RESET, handle_ball_event, ball));
}

static void handle_ball_event(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ARK_UNUSED(bus);

    ES_CTX_EXPECT(ctx, ball_t);
    ball_t *ball = ES_CTX_PTR(ctx, ball_t);

    switch (es_get_event_type(event)) {
        case ES_EV_BALL_LAUNCHED:
            ball->launched = true;
            break;
        case ES_EV_BALL_HIT_WALL: {
            ES_EV_EXPECT(event, Vector2);
            const Vector2 normal = ES_EV_VAL(event, Vector2);
            ball_handle_walls_collision(ball, normal);
            break;
        }
        case ES_EV_BALL_HIT_PADDLE: {
            ES_EV_EXPECT(event, Rectangle);
            const Rectangle *rect = ES_EV_CPTR(event, Rectangle);
            ball_handle_paddle_collision(ball, rect);
            break;
        }
        case ES_EV_BALL_HIT_BRICK: {
            ES_EV_EXPECT(event, Rectangle);
            const Rectangle *rect = ES_EV_CPTR(event, Rectangle);
            ball_handle_rect_collision(ball, rect);
            break;
        }
        case ES_EV_BALL_RESET:
            ball_init(ball, BALL_INIT_POS, BALL_RADIUS, BALL_INIT_VEL, BALL_COLOR);
            break;
        default:
            break;
    }
}