#pragma once

#include <stdbool.h>

#include "raylib.h"

#include "event_system.h"

typedef enum {
    ASSET_SFX_BALL_BOUNCE = 0,
    ASSET_SFX_BALL_HIT,
    ASSET_SFX__COUNT
} asset_sfx_e;

typedef enum {
    ASSET_MUSIC_BG = 0,
    ASSET_MUSIC__COUNT
} asset_music_e;

typedef struct {
    bool is_sfx_init;
    Sound sfx[ASSET_SFX__COUNT];

    bool is_music_init;
    Music music[ASSET_MUSIC__COUNT];
} audio_t;

bool audio_init(audio_t *audio);
void audio_update(const audio_t *audio);
void audio_play_sfx(const audio_t *audio, asset_sfx_e sfx);
void audio_unload(const audio_t *audio);
void audio_register_events(es_event_bus_t *bus, audio_t *audio);