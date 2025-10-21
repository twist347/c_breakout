#include "util.h"

#include "raylib.h"

#include <assert.h>

const char *asset_path(const char *rel_path) {
    assert(rel_path);

    return TextFormat("%s%s%s", GetApplicationDirectory(), ASSETS_DIR, rel_path);
}
