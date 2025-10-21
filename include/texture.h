#pragma once

#include <stdbool.h>

#include "raylib.h"

typedef enum {
    ASSET_TEXTURE_BG = 0,
    ASSET_TEXTURE_MENU,
    ASSET_TEXTURE__COUNT
} asset_texture_e;

typedef struct {
    bool is_texture_init;
    Texture2D textures[ASSET_TEXTURE__COUNT];
} texture_t;

bool texture_init(texture_t *texture);
void texture_render(const texture_t *texture, asset_texture_e type);
void texture_unload(const texture_t *texture);