#include <stdlib.h>

#include "raylib.h"
#include "config.h"

#include "game.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    if (!IsWindowReady()) {
        TraceLog(LOG_ERROR, "failed to create window");
        return EXIT_FAILURE;
    }

    SetTargetFPS(TARGET_FPS);

    game_t *game = game_create();
    if (!game) {
        CloseWindow();
        return EXIT_FAILURE;
    }

    while (!WindowShouldClose()) {
        game_input(game);
        game_update(game, GetFrameTime());
        game_render(game);
    }

    game_destroy(game);
    CloseWindow();

    return EXIT_SUCCESS;
}
