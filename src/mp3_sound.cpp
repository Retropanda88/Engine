#include <SDL/SDL.h>
#include <dec.h>
#include <log.h>
#include <mp3_sound.h>


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
