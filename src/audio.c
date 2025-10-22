#include "audio.h"

#include <assert.h>
#include <stddef.h>

#include "util.h"
#include "config.h"

static void handle_audio_event(const es_event_t *event, es_event_bus_t *bus, void *ctx);

bool audio_init(audio_t *audio) {
    assert(audio);

    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "audio_init: failed to initialize audio device");
        return false;
    }

    audio->is_sfx_init = false;
    audio->sfx[ASSET_SFX_BALL_BOUNCE] = LoadSound(asset_path(ASSET_SFX_BOUNCE_PATH));
    audio->sfx[ASSET_SFX_BALL_HIT] = LoadSound(asset_path(ASSET_SFX_WALL_HIT_PATH));
    audio->is_sfx_init = IsSoundValid(audio->sfx[ASSET_SFX_BALL_BOUNCE]) || IsSoundValid(audio->sfx[ASSET_SFX_BALL_HIT]);

    audio->is_music_init = false;
    audio->music[ASSET_MUSIC_BG] = LoadMusicStream(asset_path(ASSET_MUSIC_BG_PATH));
    audio->is_music_init = IsMusicValid(audio->music[ASSET_MUSIC_BG]);

    if (audio->is_music_init) {
        PlayMusicStream(audio->music[ASSET_MUSIC_BG]);
    }

    return audio->is_sfx_init || audio->is_music_init;
}

void audio_update(const audio_t *audio) {
    assert(audio);

    if (audio->is_music_init) {
        UpdateMusicStream(audio->music[ASSET_MUSIC_BG]);
    }
}

void audio_play_sfx(const audio_t *audio, asset_sfx_e sfx) {
    assert(audio);
    assert(sfx < ASSET_SFX__COUNT);

    if (audio->is_sfx_init) {
        PlaySound(audio->sfx[sfx]);
    }
}

void audio_unload(const audio_t *audio) {
    assert(audio);

    for (size_t i = 0; i < ASSET_SFX__COUNT; ++i) {
        StopSound(audio->sfx[i]);
        UnloadSound(audio->sfx[i]);
    }

    for (size_t i = 0; i < ASSET_MUSIC__COUNT; ++i) {
        StopMusicStream(audio->music[i]);
        UnloadMusicStream(audio->music[i]);
    }

    CloseAudioDevice();
}

void audio_register_events(es_event_bus_t *bus, audio_t *audio) {
    assert(bus);
    assert(audio);

    assert(es_subscribe(bus, ES_EV_SFX_PLAY_BOUNCE, handle_audio_event, audio));
    assert(es_subscribe(bus, ES_EV_SFX_PLAY_WALL_HIT, handle_audio_event, audio));
}

static void handle_audio_event(const es_event_t *event, es_event_bus_t *bus, void *ctx) {
    assert(event);
    assert(bus);

    ARK_UNUSED(bus);

    const audio_t *audio = ctx;

    switch (es_get_event_type(event)) {
        case ES_EV_SFX_PLAY_BOUNCE:
            audio_play_sfx(audio, ASSET_SFX_BALL_BOUNCE);
            break;
        case ES_EV_SFX_PLAY_WALL_HIT:
            audio_play_sfx(audio, ASSET_SFX_BALL_HIT);
            break;
        default:
            break;
    }
}
