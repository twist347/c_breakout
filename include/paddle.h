#pragma once

#include "raylib.h"

#include "event_system.h"

typedef struct {
    Rectangle rect;
    float vel;
    int axis; // -1, 0, +1
    Color color;
} paddle_t;

void paddle_init(paddle_t *paddle, Rectangle rect, float vel, Color color);
void paddle_update(paddle_t *paddle, float dt);
void paddle_render(const paddle_t *paddle);
void paddle_register_events(es_event_bus_t *bus, paddle_t *paddle);
