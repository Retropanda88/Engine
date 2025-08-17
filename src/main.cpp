#include <SDL/SDL.h>
#include <Sprite.h>
#include <gfxtexture.h>
#include <video.h>
#include <gfxFont.h>
#include <sfx.h>
#include <surface_Tools.h>
#include <sheet_bmp.h>

// Direcciones para animar sprite (se reservan aunque no uses todas aún)
enum { UP, DOWN, LEFT, RIGHT, MAX_SPRITES };

// Resolución base de tu motor
const int SCREEN_WIDTH  = 320;
const int SCREEN_HEIGHT = 240;

// Recursos globales
Sprite* sheet = nullptr;
Sprite* spr[MAX_SPRITES] = { nullptr };
gfxFont font;
GfxTexture work_texture;
SDL_Surface* p = nullptr;
sfxMusic music;

int main(int argc, char** argv)
{
    // Inicializa el sistema
    if (Init_Sistem("Engine v1.0") != 0) {
        fprintf(stderr, "Error: al iniciar el sistema\n");
        return EXIT_FAILURE;
    }

    // Inicializa el video
    if (Set_Video(SCREEN_WIDTH, SCREEN_HEIGHT) != 0) {
        fprintf(stderr, "Error: al iniciar el video\n");
        return EXIT_FAILURE;
    }
    
    sfx_init();
    sfx_setmusicvolume(100);
    sfx_setsoundvolume(80);

    // Inicializa tipografía
    font.init();

    // Superficie temporal para texto
    p = create_surface(80, 20, 32);
    if (!p) {
        fprintf(stderr, "Error: no se pudo crear la superficie\n");
        return EXIT_FAILURE;
    }

    // Escribe texto en la superficie
    font.draw(p, MMX_FONT, 0, 0, "Hello Engine");

    // Sprite desde datos embebidos
    sheet = new Sprite((u8*)sheet_data, sheet_size, 1, 1);
    sheet->setTransparency(255, 0, 255);

    // Configuración de textura de trabajo
    work_texture.init(p->w, p->h);
    work_texture.fill(0, 0, 0);
    work_texture.set_position(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    work_texture.set_surface(p, 0, 12);
    work_texture.set_scale(0.5f);
    work_texture.set_rotation(20.0f);

    // Variables de animación
    float rotation = 0.0f;
    float rotation_step = 0.6f;
    float scale = 1.0f;
    float scale_step = 0.03f;
    
    music.load("data/music2.wav");
    music. play(true,true);

    // Loop principal
    bool running = true;
    while (running)
    {
        // Eventos (ejemplo: salir con ESC o cerrar ventana)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        // Limpia pantalla
        SDL_FillRect(vram, nullptr, 0x00000000);

        // Actualiza animación
        work_texture.set_rotation(rotation);
        work_texture.set_scale(scale);
        work_texture.rotozoom();

        // Dibuja objetos
        //sheet->draw(vram, 0, 0);
        work_texture.render(vram);

        // Renderiza al frame buffer
        Render();

        // Actualiza variables
        rotation += rotation_step;
        scale += scale_step;

        // Rebotes en límites
        if (scale < 0.5f) scale_step = 0.03f;
        if (scale > 9.0f) scale_step = -0.03f;
        if (rotation > 360.0f) rotation_step = -0.6f;
        if (rotation < 0.0f)   rotation_step = 0.6f;

        // Control de FPS
        Fps_sincronizar(20); // más fluido que 10
    }

    // Limpieza
    delete sheet;
    if (p) SDL_FreeSurface(p);

    off_video();
    shoutdown_sistem();
    sfx_close();

    return EXIT_SUCCESS;
}