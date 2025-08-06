#ifndef GFXTEXTURE_H
#define GFXTEXTURE_H

#include <SDL/SDL.h>
typedef unsigned int u32;
typedef unsigned char u8;

class GfxTexture {
public:
    GfxTexture();
    ~GfxTexture();

    bool init(int w, int h);
    bool load_image(const char* filename); // requiere SDL_image
    void fill(u8 r, u8 g, u8 b);
    void render(SDL_Surface* dst);

    void set_position(int x, int y);
    void set_alpha(u8 alpha);

private:
    SDL_Surface* surface;
    int x, y;
};

#endif
