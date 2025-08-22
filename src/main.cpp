#include <SDL/SDL.h>
#include <Sprite.h>
#include <gfxtexture.h>
#include <video.h>
#include <gfxFont.h>
#include <sfx.h>
#include <surface_Tools.h>
#include <sheet_bmp.h>
#include <log.h>
#include <mp3_sound.h>

// Direcciones para animar sprite
enum
{ UP, DOWN, LEFT, RIGHT, MAX_SPRITES };

// Resolución base del motor
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

// Recursos globales
Sprite *sheet = nullptr;
Sprite *spr[MAX_SPRITES] = { nullptr };

gfxFont font;
GfxTexture work_texture;
SDL_Surface *p = nullptr;
mp3Music music;

int main(int argc, char **argv)
{
	// Inicializa sistema
	if (Init_Sistem("Engine v1.0") != 0)
	{
		fprintf(stderr, "Error: al iniciar el sistema\n");
		return EXIT_FAILURE;
	}

	// Inicializa video
	if (Set_Video(SCREEN_WIDTH, SCREEN_HEIGHT) != 0)
	{
		fprintf(stderr, "Error: al iniciar el video\n");
		return EXIT_FAILURE;
	}

	Init_Log();

	// Audio
	Audio_SetMusicVolume(100);
	Audio_SetSfxVolume(80);

	// Tipografía
	font.init();

	// Superficie temporal para texto
	p = create_surface(160, 20, 32);
	if (!p)
	{
		fprintf(stderr, "Error: no se pudo crear la superficie\n");
		return EXIT_FAILURE;
	}
	font.draw(p, MMX_FONT, 0, 0, "Hello Engine mp3 music");

	// Sprite desde datos embebidos
	sheet = new Sprite((u8 *) sheet_data, sheet_size, 1, 1);
	sheet->setTransparency(255, 0, 255);

	// Configuración de textura de trabajo
	work_texture.load_from_surface(p);
	work_texture.set_position(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	work_texture.set_scale(0.5f);
	work_texture.set_rotation(20.0f);

	// Variables de animación
	float rotation = 0.0f, rotation_step = 0.6f;
	float scale = 1.0f, scale_step = 0.03f;
	char alpha = 0, alpha_step = 1;

	Write_Log("[INFO] Loading MP3 music...");
	Write_Log("[INFO] Rotation animation enabled");
	Write_Log("[INFO] Scale animation enabled");
	Write_Log("[INFO] Alpha blending enabled");;
	
	music.load("data/music.mp3");
	music.play(true);

	// Loop principal
	bool running = true;
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
			if (event.type == SDL_KEYDOWN)
				if(event.key.keysym.sym == SDLK_a){
					music.fadeout(800);
				}
				if(event.key.keysym.sym == SDLK_b && !music.isPlaying()){
					music.play(true);
				}
		}

		// Limpia pantalla
		SDL_FillRect(vram, nullptr, 0x00000000);

		// Actualiza animación
		work_texture.set_rotation(rotation);
		work_texture.set_scale(scale);
		work_texture.set_alpha(alpha);
		work_texture.rotozoom();

		// Renderiza
		work_texture.render(vram);
		print_log();
		Render();

		// Actualiza variables
		rotation += rotation_step;
		scale += scale_step;
		alpha += alpha_step;

		// Rebotes en límites
		if (scale < 0.5f)
			scale_step = 0.03f;
		if (scale > 9.0f)
			scale_step = -0.03f;
		if (rotation > 360.0f)
			rotation_step = -0.6f;
		if (rotation < 0.0f)
			rotation_step = 0.6f;
		if (alpha < 1)
			alpha_step = 1;
		if (alpha > 254)
			alpha_step = -1;

		// Control de FPS
		Fps_sincronizar(20);
		update_log_scroll();
	}

	// Limpieza
	delete sheet;
	if (p)
		SDL_FreeSurface(p);
	off_video();
	Audio_Shutdown();

	return EXIT_SUCCESS;
}
