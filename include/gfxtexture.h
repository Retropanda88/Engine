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
    void fill_checkerboard(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, int block_size);
    void fill_checkerboard_colors(u32 c1, u32 c2, int block_size);



    void render(SDL_Surface* dst);

    void set_position(int x, int y);
    void set_alpha(u8 alpha);

private:
    SDL_Surface* surface;
    int x, y;
    int w;
    int h;
    u8 alpha;
};

#endif
