#include "texture.h"

#include <assert.h>
#include <stddef.h>

#include "config.h"
#include "util.h"

bool texture_init(texture_t *texture) {
    assert(texture);

    texture->is_texture_init = false;
    texture->textures[ASSET_TEXTURE_BG] = LoadTexture(asset_path(ASSET_TEXTURE_BG_STARFIELD_PATH));
    texture->textures[ASSET_TEXTURE_MENU] = LoadTexture(asset_path(ASSET_TEXTURE_MENU_AURORA_PATH));
    texture->is_texture_init = IsTextureValid(texture->textures[ASSET_TEXTURE_BG]) && IsTextureValid(
                                   texture->textures[ASSET_TEXTURE_MENU]);

    return texture->is_texture_init;
}

void texture_render(const texture_t *texture, asset_texture_e type) {
    assert(texture);
    assert(type < ASSET_TEXTURE__COUNT);

    if (texture->is_texture_init) {
        const Texture2D *t = &texture->textures[type];
        const Rectangle src = {0.f, 0.f, (float) t->width, (float) t->height};
        const Rectangle dst = {0.f, 0.f, (float) SCREEN_WIDTH, (float) SCREEN_HEIGHT};
        DrawTexturePro(*t, src, dst, (Vector2){0.f, 0.f}, 0.f, WHITE);
    }
}

void texture_unload(const texture_t *texture) {
    assert(texture);

    for (size_t i = 0; i < ASSET_TEXTURE__COUNT; ++i) {
        UnloadTexture(texture->textures[i]);
    }
}
