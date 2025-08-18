#include <sfx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dec.h>
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

/* ============================
   Clase mp3Music
   ============================ */

mp3Music::mp3Music() {
    playing = false;
    paused = false;
    loop = false;
    audioOpened = false;
}

mp3Music::~mp3Music() {
    stop();
    if (audioOpened) SDL_CloseAudio();
    Decoder_Cleanup();
}

bool mp3Music::load(const char* filename) {
    stop();
    Decoder_Cleanup();
    
       // Guardar el path
    strncpy(filePath, filename, sizeof(filePath)-1);
    filePath[sizeof(filePath)-1] = '\0';

    if (!Decoder_Init(filePath)) {
        Write_Log("mp3Music: Error cargando %s\n", filePath);
        return false;
    }


    PCMBuffer temp;
    if (!Decoder_GetNextPCM(&temp)) {
        Write_Log("mp3Music: Error al decodificar primer frame\n");
        return false;
    }

    // Configurar SDL_Audio
    SDL_AudioSpec spec;
    spec.freq = temp.samplerate;
    spec.format = AUDIO_S16SYS;
    spec.channels = temp.channels;
    spec.samples = 512; // buffer interno
    spec.callback = audioCallback;
    spec.userdata = this;

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        Write_Log("mp3Music: Error abriendo audio: %s\n", SDL_GetError());
        return false;
    }

    audioOpened = true;
    return true;
}

void mp3Music::play(bool loopFlag) {
    if (!audioOpened) return;

    loop = loopFlag;
    playbackPos = 0;
    playing = true;
    paused = false;

    SDL_PauseAudio(0); // inicia reproducción
}

void mp3Music::pause() {
    if (!playing) return;
    paused = !paused;
    if (paused)
        SDL_PauseAudio(1);
    else
        SDL_PauseAudio(0);
}

void mp3Music::stop() {
    if (playing) {
        playing = false;
        paused = false;
        SDL_PauseAudio(1);
        playbackPos = 0;
        Decoder_Cleanup();
    }
}

void mp3Music::fadeout(int ms) {
    if (playing) {
        // Aquí podrías implementar reducción gradual del volumen
        stop();
    }
}

void mp3Music::reset() {
    stop();
    if (!Decoder_GetNextPCM(&currentPCM))
        Write_Log("mp3Music: Error reiniciando reproducción\n");
    playbackPos = 0;
}

int mp3Music::isPlaying() {
    return playing && !paused;
}

/* ============================
   Callback SDL
   ============================ */
void mp3Music::audioCallback(void* userdata, Uint8* stream, int len) {
    mp3Music* music = (mp3Music*) userdata;
    if (!music || !music->playing) {
        memset(stream, 0, len);
        return;
    }

    int16_t* out = (int16_t*) stream;
    int samples_needed = len / sizeof(int16_t);

    while (samples_needed > 0 && music->playing && !music->paused) {
        if (music->playbackPos >= (int)(music->currentPCM.length * music->currentPCM.channels)) {
            if (!Decoder_GetNextPCM(&music->currentPCM)) {
                if (music->loop) {
                    Decoder_Cleanup();
                    Decoder_Init(music->filePath); // Podrías guardar el path
                    if (!Decoder_GetNextPCM(&music->currentPCM)) {
                        music->playing = false;
                        break;
                    }
                    music->playbackPos = 0;
                } else {
                    music->playing = false;
                    break;
                }
            } else {
                music->playbackPos = 0;
            }
        }

        int available_samples = (music->currentPCM.length * music->currentPCM.channels) - music->playbackPos;
        int samples_to_copy = (samples_needed < available_samples) ? samples_needed : available_samples;

        memcpy(out, &music->currentPCM.samples[music->playbackPos], samples_to_copy * sizeof(int16_t));
        out += samples_to_copy;
        music->playbackPos += samples_to_copy;
        samples_needed -= samples_to_copy;
    }

    if (!music->playing && samples_needed > 0)
        memset(out, 0, samples_needed * sizeof(int16_t));
}