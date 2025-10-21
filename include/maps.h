#pragma once

#include "config.h"

#define MAP_ROWS(map) (sizeof(map) / sizeof((map)[0]))
#define MAP_COLS(map) (sizeof((map)[0]) / sizeof((map)[0][0]))

#define CHECK_MAP(map, R, C) \
_Static_assert(MAP_ROWS(map) == (R), #map " rows mismatch"); \
_Static_assert(MAP_COLS(map) == (C), #map " cols mismatch")

#define BRICK_SYMBOL_EMPTY    ' '
#define BRICK_SYMBOL_LIGHT    '1'
#define BRICK_SYMBOL_MEDIUM   '2'
#define BRICK_SYMBOL_HEAVY    '3'

static const char map1[BRICKS_ROWS][BRICKS_COLS] = {
    "1111111",
    "1111111",
    "3333333",
    "2222222",
};

static const char map2[BRICKS_ROWS][BRICKS_COLS] = {
    "   1   ",
    "  111  ",
    " 33333 ",
    "  222  ",
};

static const char (*maps[])[BRICKS_COLS] = {
    map1,
    map2,
};

CHECK_MAP(map1, BRICKS_ROWS, BRICKS_COLS);
CHECK_MAP(map2, BRICKS_ROWS, BRICKS_COLS);
