/*
 * libGPP-Engine - A lightweight static game engine for retro consoles.
 * Copyright (c) 2025 Andrés Ruiz Pérez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or version 3.
 * https://www.gnu.org/licenses/
 */


#include <pixel.h>
#include <types.h>


void pixel(SDL_Surface *src, int x, int y, u32 c){

    if (!src || x < 0 || x >= src->w || y < 0 || y >= src->h)
        return;

    // Asegúrate de bloquear el surface si es necesario
    if (SDL_MUSTLOCK(src)) {
        if (SDL_LockSurface(src) < 0) return;
    }

    Uint32 *pixels = (Uint32 *)src->pixels;
    int pitch = src->pitch / 4; // número de Uint32 por fila

    pixels[y * pitch + x] = c;

    if (SDL_MUSTLOCK(src)) {
        SDL_UnlockSurface(src);
    }
}


Uint32 get_pixel(SDL_Surface *src, int x, int y) {
    if (!src || x < 0 || x >= src->w || y < 0 || y >= src->h)
        return 0;  // Devuelve 0 si la posición es inválida

    // Asegúrate de bloquear el surface si es necesario
    if (SDL_MUSTLOCK(src)) {
        if (SDL_LockSurface(src) < 0) return 0;
    }

    Uint32 *pixels = (Uint32 *)src->pixels;
    int pitch = src->pitch / 4; // Número de Uint32 por fila

    Uint32 color = pixels[y * pitch + x];

    if (SDL_MUSTLOCK(src)) {
        SDL_UnlockSurface(src);
    }

    return color;
}

