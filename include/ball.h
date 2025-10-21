#pragma once

#include <stdbool.h>

#include "raylib.h"

#include "event_system.h"

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float radius;
    bool launched; // follows paddle until launch
    Color color;
} ball_t;

void ball_init(ball_t *ball, Vector2 pos, float radius, Vector2 vel, Color color);
void ball_update(ball_t *ball, float dt);
void ball_follow_paddle(ball_t *ball, const Rectangle *paddle_rect);

bool ball_check_rect_collision(const ball_t *ball, const Rectangle *rect);
void ball_handle_rect_collision(ball_t *ball, const Rectangle *rect);

void ball_render(const ball_t *ball);

bool ball_check_bottom_collision(const ball_t *ball, Vector2 screen_res);

bool ball_check_left_wall_collision(const ball_t *ball, Vector2 screen_res);
bool ball_check_right_wall_collision(const ball_t *ball, Vector2 screen_res);
bool ball_check_top_wall_collision(const ball_t *ball, Vector2 screen_res);

void ball_handle_walls_collision(ball_t *ball, Vector2 normal);

bool ball_check_paddle_collision(const ball_t *ball, const Rectangle *paddle_rect);
void ball_handle_paddle_collision(ball_t *ball, const Rectangle *paddle_rect);

void ball_register_events(es_event_bus_t *bus, ball_t *ball);