
/*
 * libGPP-Engine - A lightweight static game engine for retro consoles.
 * Copyright (c) 2025 Andrés Ruiz Pérez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or version 3.
 * https://www.gnu.org/licenses/
 */

#include <stdio.h>
#include <SDL/SDL.h>
#include <types.h>
#include <video.h>
#include <font.h>

//vram 
SDL_Surface *vram = NULL;

//framebuffer
u32 *fb = NULL;


/**
 * @brief Inicializa el sistema con un mensaje opcional.
 *
 * Esta función se encarga de realizar la inicialización del sistema. 
 * Toma un parámetro `msg` de tipo puntero a `int` que puede representar
 * información adicional para la inicialización (por ejemplo, un código de configuración
 * o una señal para un modo específico de inicialización). 
 * El comportamiento exacto de la función depende de cómo se implemente el parámetro `msg`.
 *
 * @param msg Puntero a un valor char que puede contener información relevante para la inicialización.
 *            El significado exacto de este valor depende de la implementación de la función.
 *            Puede ser `NULL` o contener un valor válido según sea necesario.
 *
 * @return 0 si la inicialización fue exitosa, o un código de error diferente de 0 si hubo algún fallo.
 *
 * @note Esta función puede requerir ciertos recursos del sistema para ser ejecutada correctamente, 
 *       como memoria o configuraciones previas. Asegúrese de que todos los requisitos previos estén
 *       cumplidos antes de llamarla.
 */
int Init_Sistem(const char *msg){

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0){
		printf("error: %s",SDL_GetError());
		return -1;
	}

	printf("\n%s\n",msg);
	fontsize(8, 8);

	return 0;

}

/**
 * @brief Configura la resolución de video.
 *
 * Esta función establece la resolución de video del sistema. Toma los valores de
 * `width` (ancho) y `height` (alto) en píxeles, que determinan las dimensiones
 * de la ventana o la pantalla en la que se renderiza el contenido.
 *
 * @param width Ancho de la resolución en píxeles.
 * @param height Alto de la resolución en píxeles.
 *
 * @return 0 si la configuración de la resolución fue exitosa, o un código de error
 *         diferente de 0 si hubo algún problema al configurar la resolución.
 *
 * @note Asegúrese de que los valores de `width` y `height` sean válidos y compatibles
 *       con la pantalla o el dispositivo en el que se está ejecutando el programa.
 *       Esta función puede ajustar la configuración de la pantalla o la ventana de acuerdo
 *       con el hardware disponible.
 */
int Set_Video(int width, int height){

	vram = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
	if(!vram){
		printf("error: %s\n",SDL_GetError());
		return -1;
	}

	SDL_ShowCursor(SDL_FALSE);

	fb = (u32*)vram->pixels;

	return 0;

}


/**
 * @brief Obtiene las dimensiones de la resolución de video actual.
 *
 * Esta función obtiene el ancho y el alto de la resolución de video actual y los almacena
 * en los punteros `width` y `height` proporcionados.
 *
 * @param width Puntero a una variable donde se almacenará el ancho de la resolución en píxeles.
 * @param height Puntero a una variable donde se almacenará el alto de la resolución en píxeles.
 *
 * @note Asegúrese de que las variables a las que apuntan `width` y `height` sean accesibles
 *       y tengan espacio suficiente para almacenar los valores de la resolución.
 */
void get_wh_video_mode(int *width, int *height){

	if(!vram){
		printf("not set video mode\n");
		*width = 0;
		*height = 0;
	}

	*width = vram->w;
	*height = vram->h;

}


/**
 * @brief Apaga el sistema de manera segura.
 *
 * Esta función realiza un apagado controlado del sistema. Cierra todos los procesos activos
 * y libera recursos utilizados para asegurar que el sistema se apague correctamente.
 *
 * @note Es importante llamar a esta función antes de apagar el sistema o reiniciar la máquina,
 *       para evitar la pérdida de datos o daños a los archivos abiertos.
 */
void shoutdown_sistem(){
	SDL_Quit();

}


/**
 * @brief Apaga el modo de video.
 *
 * Esta función desactiva el modo de video, liberando los recursos relacionados con la
 * salida de video (por ejemplo, la memoria de la pantalla o la configuración de la pantalla).
 *
 * @note Esta función es útil cuando se desea liberar los recursos de video antes de apagar
 *       el sistema o cambiar la configuración de video.
 */
void off_video(){
	SDL_FreeSurface(vram);
	fb = NULL;
}


/**
 * @brief Renderiza los gráficos en la pantalla.
 *
 * Esta función realiza el proceso de renderizado, actualizando la pantalla con
 * los elementos gráficos que han sido procesados o modificados. Generalmente,
 * esta función es llamada en cada ciclo del juego o aplicación para mostrar
 * los cambios en la interfaz de usuario o el entorno gráfico.
 *
 * @note Asegúrese de que todos los objetos gráficos y recursos hayan sido preparados
 *       antes de llamar a esta función. Esta función puede implicar una actualización
 *       de la superficie de video o del framebuffer.
 */
void Render(){
	SDL_Flip(vram);
}

/**
 * @brief Limpia la pantalla.
 *
 * Esta función limpia la pantalla, eliminando cualquier gráfico o contenido visual
 * previamente renderizado. Esto se puede usar, por ejemplo, para borrar la pantalla
 * al comienzo de un nuevo ciclo de renderizado, asegurando que solo se vean los elementos
 * gráficos actuales.
 *
 * @note Dependiendo de la implementación, esta función podría borrar el framebuffer
 *       o la superficie de video asociada.
 */
void cls() {
    int i;
    // Limpia toda la superficie poniendo todos los píxeles a 0x00000000 (negro)
    for (i = 0; i < vram->w * vram->h; i++) {
        fb[i] = 0x00000000;  // Asigna el color negro (0x00000000) a cada píxel
    }
}

/**
 * @brief Limpia la pantalla con un color específico.
 *
 * Esta función limpia la pantalla y la rellena con un color sólido, especificado
 * mediante los valores `r`, `g`, `b` correspondientes a los componentes rojo, verde
 * y azul. Los valores deben estar en el rango de 0 a 255.
 *
 * @param r Componente rojo del color, en el rango de 0 a 255.
 * @param g Componente verde del color, en el rango de 0 a 255.
 * @param b Componente azul del color, en el rango de 0 a 255.
 *
 * @note Esta función puede ser útil cuando se necesita un fondo de color específico
 *       antes de realizar el renderizado, como al iniciar una escena o cuando se
 *       desea borrar la pantalla con un color particular.
 */
void cls_rgb(u8 r, u8 g, u8 b){
	SDL_FillRect(vram,0,SDL_MapRGB(vram->format,r,g,b));
}

/**
 * @brief Crea un color en formato ARGB a partir de los componentes RGB.
 *
 * Esta función toma tres valores enteros en el rango de 0 a 255 para los componentes
 * rojo, verde y azul, y devuelve un valor de color de 32 bits en formato ARGB, 
 * donde el canal alfa es 255 (totalmente opaco).
 *
 * @param r Componente rojo del color, en el rango de 0 a 255.
 * @param g Componente verde del color, en el rango de 0 a 255.
 * @param b Componente azul del color, en el rango de 0 a 255.
 *
 * @return El valor del color en formato ARGB (A=255, R=r, G=g, B=b).
 */
u32 color_rgb(u8 r, u8 g, u8 b) {
    return SDL_MapRGB(vram->format,r,g,b);
}

void Fps_sincronizar(int frecuencia)
{
	static int t;
	static int temp;
	static int t1 = 0;

	t = SDL_GetTicks();
	if (t - t1 >= frecuencia)
	{
		temp = (t - t1) / frecuencia;
		t1 += temp * frecuencia;
	}
	else
	{
		SDL_Delay(frecuencia - (t - t1));
		t1 += frecuencia;
	}
}

