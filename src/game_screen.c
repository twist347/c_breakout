#include "game_screen.h"

#include <assert.h>

void game_screen_change(game_screen_e *from, game_screen_e to) {
    assert(from);
    assert(to < GAME_SCREEN__COUNT);

    *from = to;
}
