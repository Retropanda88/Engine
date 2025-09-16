#ifndef MP3_SOUND
#define MP3_SOUND

#include <mad.h>
#include <string>
#include <cstdio>
#include <dec.h>


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



#endif