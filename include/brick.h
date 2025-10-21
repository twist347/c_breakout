#pragma once

#include "raylib.h"

#include "event_system.h"

typedef enum {
    BRICK_TYPE_LIGHT = 0,
    BRICK_TYPE_MEDIUM,
    BRICK_TYPE_HEAVY,
    BRICK_TYPE__COUNT
} brick_type_e;

typedef struct {
    Rectangle rect;
    bool active;
    brick_type_e type;
    Color color;
    int lives;
} brick_t;

void brick_init(brick_t *brick, Rectangle rect, bool active, brick_type_e type);
void brick_render(const brick_t *brick);
void brick_render_grid(const brick_t *bricks);
int brick_type_to_score(brick_type_e type);
void brick_register_events(es_event_bus_t *bus, brick_t *bricks);
