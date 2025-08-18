#ifndef DEC_H
#define DEC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int16_t* samples;       // buffer PCM (interleaved L-R)
    unsigned int length;    // número de muestras por canal en el buffer
    unsigned int channels;  // 1 o 2 canales
    unsigned int samplerate;// frecuencia de muestreo
} PCMBuffer;

// Inicializa el decoder con el archivo MP3
bool Decoder_Init(const char* filename);

// Decodifica el siguiente frame y llena el buffer PCM
// Devuelve true si hay datos, false si llegó al final
bool Decoder_GetNextPCM(PCMBuffer* outBuffer);

// Libera recursos
void Decoder_Cleanup();

#ifdef __cplusplus
}
#endif

#endif // DECODER_H