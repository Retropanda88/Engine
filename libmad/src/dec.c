#include <dec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mad.h>

#define INPUT_BUFFER_SIZE 8192
#define MAD_BUFFER_GUARD 8

static FILE* mp3File = NULL;
static unsigned char inputBuffer[INPUT_BUFFER_SIZE + MAD_BUFFER_GUARD];
static size_t inputSize = 0;

static struct mad_stream stream;
static struct mad_frame frame;
static struct mad_synth synth;

static bool initialized = false;

// Convierte mad_fixed_t a int16_t PCM con saturación
static int16_t mad_fixed_to_pcm(mad_fixed_t sample) {
    sample += (1L << (MAD_F_FRACBITS - 16));
    if (sample >= MAD_F_ONE)
        sample = MAD_F_ONE - 1;
    else if (sample < -MAD_F_ONE)
        sample = -MAD_F_ONE;
    return (int16_t)(sample >> (MAD_F_FRACBITS + 1 - 16));
}

bool Decoder_Init(const char* filename) {
	
    mp3File = fopen(filename, "rb");
    if (!mp3File) {
        perror("Error abriendo archivo MP3");
        return false;
    }

    memset(inputBuffer, 0, sizeof(inputBuffer));
    inputSize = fread(inputBuffer, 1, INPUT_BUFFER_SIZE, mp3File);

    mad_stream_init(&stream);
    mad_frame_init(&frame);
    mad_synth_init(&synth);

    mad_stream_buffer(&stream, inputBuffer, inputSize);
    stream.error = MAD_ERROR_NONE;

    initialized = true;
    return true;
}

bool Decoder_GetNextPCM(PCMBuffer* outBuffer) {
    if (!initialized) return false;

    while (1) {
        if (mad_frame_decode(&frame, &stream)) {
            if (MAD_RECOVERABLE(stream.error)) {
                // Error recuperable, saltamos
                continue;
            } else if (stream.error == MAD_ERROR_BUFLEN) {
                // Fin buffer, leer más
                size_t remaining = inputSize - (stream.next_frame - inputBuffer);
                memmove(inputBuffer, stream.next_frame, remaining);
                size_t bytesRead = fread(inputBuffer + remaining, 1, INPUT_BUFFER_SIZE - remaining, mp3File);
                if (bytesRead == 0) return false; // fin archivo

                inputSize = remaining + bytesRead;
                mad_stream_buffer(&stream, inputBuffer, inputSize);
                continue;
            } else {
                // Error fatal
                return false;
            }
        }

        mad_synth_frame(&synth, &frame);

        // Preparamos el buffer PCM
        unsigned int nsamples = synth.pcm.length;
        unsigned int nchannels = synth.pcm.channels;

        // Alocamos o reutilizamos memoria para samples intercalados
        static int16_t* pcmData = NULL;
        static size_t pcmCapacity = 0;
        size_t needed = nsamples * nchannels * sizeof(int16_t);
        if (pcmCapacity < needed) {
            free(pcmData);
            pcmData = malloc(needed);
            pcmCapacity = needed;
        }

        for (unsigned int i = 0; i < nsamples; i++) {
            int16_t left = mad_fixed_to_pcm(synth.pcm.samples[0][i]);
            int16_t right = nchannels == 2 ? mad_fixed_to_pcm(synth.pcm.samples[1][i]) : left;

            pcmData[i * nchannels + 0] = left;
            if (nchannels == 2) pcmData[i * nchannels + 1] = right;
        }

        outBuffer->samples = pcmData;
        outBuffer->length = nsamples;
        outBuffer->channels = nchannels;
        outBuffer->samplerate = frame.header.samplerate;

        return true;
    }
}

void Decoder_Cleanup() {
    if (mp3File) fclose(mp3File);
    mad_synth_finish(&synth);
    mad_frame_finish(&frame);
    mad_stream_finish(&stream);
    initialized = false;
}