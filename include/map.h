#pragma once

#include <stddef.h>

#include "config.h"

#define BRICK_SYMBOL_EMPTY    ' '
#define BRICK_SYMBOL_LIGHT    '1'
#define BRICK_SYMBOL_MEDIUM   '2'
#define BRICK_SYMBOL_HEAVY    '3'

extern const char (*const maps[])[BRICKS_COLS];
extern const size_t maps_count;

const char (*get_map(size_t idx))[BRICKS_COLS];
