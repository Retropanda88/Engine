#ifndef SFX_H
#define SFX_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <mad.h>
#include <string>
#include <cstdio>
#include <dec.h>

#define NUM_SOUND_CHANNELS 16

/* ============================
   Funciones globales de Audio
   ============================ */
bool Audio_Init();
void Audio_Shutdown();
void Audio_StopAll();
void Audio_SetMusicVolume(int volume);
void Audio_SetSfxVolume(int volume);

/* ============================
   Clase: sfxSound (efectos WAV)
   ============================ */
class sfxSound
{
public:
    sfxSound();
    ~sfxSound();

    bool init(const char* filename);

    int play();
    int playloop(int iLoop);
    void stop();
    void fadeout(int ms);
    void sfx_pause();

    void resetpause() { paused = false; }
    void reset();
    bool isready() { return ready; }
    int isplaying();

    void clearchannel();

private:
    Mix_Chunk *sfx;
    int channel;
    bool paused;
    bool ready;
    int starttime;
    short instances;
};

/* ============================
   Clase: sfxMusic (música WAV/OGG/MOD)
   ============================ */
class sfxMusic
{
public:
    sfxMusic();
    ~sfxMusic();

    bool load(const char* filename);

    void play(bool fPlayonce, bool fResume);
    void stop();
    void sfx_pause();
    void fadeout(int ms);

    void resetpause() { paused = false; }
    void reset();
    bool isready() { return ready; }
    int isplaying();

private:
    Mix_Music *music;
    bool paused;
    bool ready;
};

/* ============================
   Clase: mp3Music (usando libmad)
   ============================ */


class mp3Music {
public:
    mp3Music();
    ~mp3Music();

    // Carga un archivo MP3
    bool load(const char* filename);

    // Reproduce, con opción de loop
    void play(bool loopFlag = false);

    // Pausa/reanuda reproducción
    void pause();

    // Detiene la reproducción
    void stop();

    // Fade out simulado
    void fadeout(int ms);

    // Reinicia reproducción desde el principio
    void reset();

    // Devuelve true si está reproduciendo
    int isPlaying();

private:
    PCMBuffer currentPCM;
    int playbackPos;
    bool playing;
    bool paused;
    bool loop;
    bool audioOpened;
    char filePath[512]; // guarda la ruta del MP3

    // Callback SDL para rellenar el buffer de audio
    static void audioCallback(void* userdata, Uint8* stream, int len);
};





#endif // SFX_H