#include "map.h"

#include <assert.h>

#define MAP_ROWS(map) (sizeof(map) / sizeof((map)[0]))
#define MAP_COLS(map) (sizeof((map)[0]) / sizeof((map)[0][0]))

#define CHECK_MAP(map, R, C)                                   \
_Static_assert(MAP_ROWS(map) == (R), #map " rows mismatch");   \
_Static_assert(MAP_COLS(map) == (C), #map " cols mismatch")

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

const char (*const maps[])[BRICKS_COLS] = {
    map1,
    map2,
};

const size_t maps_count = sizeof(maps) / sizeof(maps[0]);

const char (*get_map(size_t idx))[BRICKS_COLS] {
    assert(idx < maps_count);

    return maps[idx];
}

CHECK_MAP(map1, BRICKS_ROWS, BRICKS_COLS);
CHECK_MAP(map2, BRICKS_ROWS, BRICKS_COLS);
