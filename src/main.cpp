#include <SDL/SDL.h>
#include "gfxtexture.h"

/*motor de videojuegos multiplataformas*/

const int SCREEN_W = 320;
const int SCREEN_H = 240;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_SWSURFACE);
    SDL_ShowCursor(SDL_DISABLE);

    GfxTexture tex;
    tex.init(50, 50);
    tex.set_position(20, 20);
    tex.fill(255, 0, 0); // rojo

    while (1) {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0)); // limpiar pantalla

        tex.render(screen);
        SDL_Flip(screen);
        SDL_Delay(16);
    }

    SDL_Quit();
    return 0;
}
