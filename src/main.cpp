#include <SDL/SDL.h>
#include <Sprite.h>
#include <gfxtexture.h>
#include <video.h>
#include <gfxFont.h>


#include <sheet_bmp.h>

enum
{ up, down, left, right, max_sprites };

const int w_screen = 320;
const int h_screen = 240;

Sprite *sheet = NULL;
Sprite *spr[max_sprites];
gfxFont font;

GfxTexture work_texture;


int main(int argc, char **argv)
{
	if(Init_Sistem("Engine v1.0") != 0){
		printf("error: al iniciar el sistema\n");
		return 1;
	}

   if(Set_Video(w_screen,h_screen) != 0){
   	printf("error: al iniciar el video\n");
		return 1;
   }
   
   font.init();
   

	sheet = new Sprite((u8 *) sheet_data, sheet_size, 1, 1);
	sheet->setTransparency(255, 0, 255);

	work_texture.init(w_screen, h_screen);
	work_texture.fill(255, 0, 0);
	work_texture.set_position(w_screen / 2, h_screen / 2);
	work_texture.set_scale(0.5f);
	work_texture.set_rotation(20);
	float r=0;
	while (1)
	{
		SDL_FillRect(vram,0,0xffffffff);
		work_texture.set_rotation(r);
		work_texture.rotozoom();
		work_texture.render(vram);
		sheet->draw(vram, 0, 0);
		font.draw(vram, MMX_FONT, 100, 100,"hello engine");
        Render();
        r++;
        Fps_sincronizar(10);
	}
    off_video();
    shoutdown_sistem();
	return 0;
}
