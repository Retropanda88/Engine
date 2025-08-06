#include "gfxtexture.h"
#include <SDL/SDL_image.h> // si quieres soporte para imágenes
#include <stdio.h>

GfxTexture::GfxTexture() : surface(NULL), x(0), y(0) {}

GfxTexture::~GfxTexture() {
    if (surface) SDL_FreeSurface(surface);
}

bool GfxTexture::init(int w, int h) {
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    if (!surface) {
        printf("SDL_CreateRGBSurface error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool GfxTexture::load_image(const char* filename) {
    SDL_Surface* temp = IMG_Load(filename);
    if (!temp) {
        printf("IMG_Load error: %s\n", IMG_GetError());
        return false;
    }

    surface = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    return surface != NULL;
}

void GfxTexture::fill(u8 r, u8 g, u8 b) {
    if (!surface) return;

    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    u32 color = SDL_MapRGB(surface->format, r, g, b);
    SDL_FillRect(surface, NULL, color);

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

void GfxTexture::render(SDL_Surface* dst) {
    if (!surface || !dst) return;
    SDL_Rect pos = { x, y, 0, 0 };
    SDL_BlitSurface(surface, NULL, dst, &pos);
}

void GfxTexture::set_position(int x, int y) {
    this->x = x;
    this->y = y;
}

void GfxTexture::set_alpha(u8 alpha) {
    if (surface) SDL_SetAlpha(surface, SDL_SRCALPHA, alpha);
}
