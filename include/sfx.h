#ifndef SFX_H
#define SFX_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>


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


#endif // SFX_H