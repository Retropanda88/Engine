#include <sfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <log.h>
#include <SDL/SDL.h>

/* ============================
   Variables globales
   ============================ */
sfxSound* g_PlayingSoundChannels[NUM_SOUND_CHANNELS];

/* ============================
   Funciones globales Audio_*
   ============================ */
bool Audio_Init() {
    if (Mix_OpenAudio(22050, AUDIO_S16, 2, 1024) < 0) {
        printf("Error al inicializar SDL_mixer: %s\n", Mix_GetError());
        return false;
    }

    Mix_AllocateChannels(NUM_SOUND_CHANNELS);

    for (short i = 0; i < NUM_SOUND_CHANNELS; i++)
        g_PlayingSoundChannels[i] = NULL;

    Mix_ChannelFinished([](int channel) {
        if (g_PlayingSoundChannels[channel]) {
            g_PlayingSoundChannels[channel]->clearchannel();
            g_PlayingSoundChannels[channel] = NULL;
        }
    });

    return true;
}

void Audio_Shutdown() {
    Mix_CloseAudio();
}

void Audio_StopAll() {
    Mix_HaltChannel(-1);
    for (short i = 0; i < NUM_SOUND_CHANNELS; i++)
        g_PlayingSoundChannels[i] = NULL;
}

void Audio_SetMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void Audio_SetSfxVolume(int volume) {
    Mix_Volume(-1, volume);
}

/* ============================
   Clase sfxSound
   ============================ */
sfxSound::sfxSound() {
    paused = false;
    ready = false;
    sfx = NULL;
    channel = -1;
    starttime = 0;
    instances = 0;
}

sfxSound::~sfxSound() {
    reset();
}

bool sfxSound::init(const char* filename) {
    if (sfx) reset();

    sfx = Mix_LoadWAV(filename);
    if (!sfx) {
        printf("Error cargando WAV %s: %s\n", filename, Mix_GetError());
        return false;
    }

    channel = -1;
    starttime = 0;
    ready = true;
    instances = 0;
    return true;
}

int sfxSound::play() {
    if (!ready) return -1;
    if (channel >= 0 && Mix_Playing(channel)) return channel;

    channel = Mix_PlayChannel(-1, sfx, 0);
    if (channel < 0) {
        printf("Error reproducir WAV: %s\n", Mix_GetError());
        return -1;
    }

    g_PlayingSoundChannels[channel] = this;
    starttime = SDL_GetTicks();
    paused = false;
    instances++;
    return channel;
}

int sfxSound::playloop(int iLoop) {
    if (!ready) return -1;

    channel = Mix_PlayChannel(-1, sfx, iLoop);
    if (channel < 0) {
        printf("Error reproducir loop WAV: %s\n", Mix_GetError());
        return -1;
    }

    g_PlayingSoundChannels[channel] = this;
    instances++;
    return channel;
}

void sfxSound::stop() {
    if (channel >= 0) {
        instances = 0;
        Mix_HaltChannel(channel);
        g_PlayingSoundChannels[channel] = NULL;
        channel = -1;
    }
}

void sfxSound::sfx_pause() {
    if (channel < 0) return;
    paused = !paused;
    if (paused) Mix_Pause(channel);
    else Mix_Resume(channel);
}

void sfxSound::fadeout(int ms) {
    if (channel >= 0 && Mix_Playing(channel)) {
        Mix_FadeOutChannel(channel, ms);
        g_PlayingSoundChannels[channel] = NULL;
        channel = -1;
    }
}

int sfxSound::isplaying() {
    return channel >= 0 && Mix_Playing(channel);
}

void sfxSound::clearchannel() {
    if (--instances <= 0) {
        instances = 0;
        channel = -1;
    }
}

void sfxSound::reset() {
    if (sfx) {
        Mix_FreeChunk(sfx);
        sfx = NULL;
    }
    ready = false;
    if (channel >= 0) g_PlayingSoundChannels[channel] = NULL;
    channel = -1;
}

/* ============================
   Clase sfxMusic
   ============================ */
sfxMusic::sfxMusic() {
    music = NULL;
    paused = false;
    ready = false;
}

sfxMusic::~sfxMusic() {
    reset();
}

bool sfxMusic::load(const char* filename) {
    if (music) reset();

    music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Error cargando música %s: %s\n", filename, Mix_GetError());
        return false;
    }

    ready = true;
    return true;
}

void sfxMusic::play(bool fPlayonce, bool fResume) {
    if (!ready) return;
    if (Mix_PlayMusic(music, fPlayonce ? 0 : -1) < 0)
        printf("Error reproducir música: %s\n", Mix_GetError());
}

void sfxMusic::stop() {
    Mix_HaltMusic();
}

void sfxMusic::sfx_pause() {
    paused = !paused;
    if (paused) Mix_PauseMusic();
    else Mix_ResumeMusic();
}

void sfxMusic::fadeout(int ms) {
    Mix_FadeOutMusic(ms);
}

void sfxMusic::reset() {
    if (music) Mix_FreeMusic(music);
    music = NULL;
    ready = false;
}

int sfxMusic::isplaying() {
    return Mix_PlayingMusic();
}
