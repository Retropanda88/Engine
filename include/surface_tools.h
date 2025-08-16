/*
 * libGPP-Engine - A lightweight static game engine for retro consoles.
 * Copyright (c) 2025 Andrés Ruiz Pérez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or version 3.
 * https://www.gnu.org/licenses/
 */

#ifndef SURFACE_TOOLS_H_
#define SURFACE_TOOLS_H_

#include <SDL/SDL.h>
#include <types.h>



#ifdef __cplusplus

extern "C" {

#endif


/**
 * @brief Carga una imagen desde un archivo.
 *
 * Esta función carga una imagen desde un archivo especificado y la convierte a un formato
 * adecuado para su uso en SDL. La imagen cargada se devuelve como un puntero a una
 * superficie SDL.
 *
 * @param file Ruta al archivo de la imagen.
 * @return Un puntero a la superficie SDL con la imagen cargada, o NULL en caso de error.
 */
SDL_Surface *load_img(const char *file);


/**
 * @brief Aplica transparencia a una superficie.
 *
 * Esta función hace que todos los píxeles de la superficie que coincidan con el color
 * especificado (r, g, b) se vuelvan transparentes. El color (r, g, b) se considera como
 * el color que será transparente.
 *
 * @param src Puntero a la superficie SDL sobre la cual se aplicará la transparencia.
 * @param r Componente rojo del color que será transparente, en el rango de 0 a 255.
 * @param g Componente verde del color que será transparente, en el rango de 0 a 255.
 * @param b Componente azul del color que será transparente, en el rango de 0 a 255.
 * @return 0 si se aplicó la transparencia correctamente, o -1 si hubo un error.
 */
int AplyTransparency(SDL_Surface *src, u8 r, u8 g, u8 b);


/**
 * @brief Dibuja una superficie en una posición específica.
 *
 * Esta función dibuja una superficie SDL en las coordenadas (x, y) de la pantalla
 * o de otra superficie de destino.
 *
 * @param src Puntero a la superficie SDL que se desea dibujar.
 * @param x Coordenada X donde se dibujará la superficie.
 * @param y Coordenada Y donde se dibujará la superficie.
 */
void draw_surface(SDL_Surface *src, int x, int y);


/**
 * @brief Crea una superficie SDL vacía.
 *
 * Esta función crea una nueva superficie de tamaño `w`x`h` con un formato y
 * banderas especificadas.
 *
 * @param w Ancho de la nueva superficie.
 * @param h Alto de la nueva superficie.
 * @param flag Banderas que especifican características adicionales de la superficie,
 *             como la transparencia o el formato.
 * @return Un puntero a la nueva superficie creada, o NULL si hubo un error.
 */
SDL_Surface *create_surface(int w, int h, Uint32 flag);


/**
 * @brief Llena una superficie con un patrón de colores alternos.
 *
 * Esta función llena una superficie con un patrón de colores alternos entre dos colores
 * `c1` y `c2`. El patrón de llenado cubrirá toda la superficie.
 *
 * @param src Puntero a la superficie SDL a llenar con el patrón.
 * @param w Ancho de la superficie.
 * @param h Alto de la superficie.
 * @param c1 Primer color del patrón (en formato ARGB).
 * @param c2 Segundo color del patrón (en formato ARGB).
 */
void fill_texture(SDL_Surface *src, int w, int h, u32 c1, u32 c2);


/**
 * @brief Llena una superficie con un patrón de colores alternos en bloques.
 *
 * Similar a `fill_texture`, pero en este caso, el patrón se llena en bloques de
 * tamaño `block_size`, alternando entre los colores `c1` y `c2`.
 *
 * @param src Puntero a la superficie SDL a llenar con el patrón.
 * @param w Ancho de la superficie.
 * @param h Alto de la superficie.
 * @param c1 Primer color del patrón (en formato ARGB).
 * @param c2 Segundo color del patrón (en formato ARGB).
 * @param block_size Tamaño de los bloques en los que se alternarán los colores.
 */
void fill_textureRect(SDL_Surface *src, int w, int h, Uint32 c1, Uint32 c2, int block_size);


/**
 * @brief Aplica un valor de opacidad (alpha) a toda una superficie.
 *
 * Esta función aplica un valor de opacidad a toda la superficie, lo que hace que la
 * imagen sea más o menos transparente según el valor `alpha`. Un valor de `alpha`
 * de 255 significa completamente opaco, y un valor de 0 significa completamente
 * transparente.
 *
 * @param src Puntero a la superficie SDL a la cual se le aplicará el valor de opacidad.
 * @param alpha Valor de opacidad a aplicar, en el rango de 0 a 255.
 */
void AplyAlpha(SDL_Surface *src, u8 alpha);


/**
 * @brief Llena una superficie con un gradiente horizontal.
 *
 * Esta función llena una superficie con un gradiente que va desde el color `color1`
 * en el borde izquierdo hasta el color `color2` en el borde derecho.
 *
 * @param surface Puntero a la superficie SDL que será llenada con el gradiente.
 * @param color1 Color inicial del gradiente (en formato ARGB).
 * @param color2 Color final del gradiente (en formato ARGB).
 */
void fill_horizontal_gradient(SDL_Surface *surface, Uint32 color1, Uint32 color2);


/**
 * @brief Llena una superficie con un gradiente vertical.
 *
 * Esta función llena una superficie con un gradiente que va desde el color `color1`
 * en la parte superior hasta el color `color2` en la parte inferior.
 *
 * @param surface Puntero a la superficie SDL que será llenada con el gradiente.
 * @param color1 Color inicial del gradiente (en formato ARGB).
 * @param color2 Color final del gradiente (en formato ARGB).
 */
void fill_vertical_gradient(SDL_Surface *surface, Uint32 color1, Uint32 color2);


/**
 * @brief Llena una superficie con un gradiente radial.
 *
 * Esta función crea un gradiente radial que se extiende desde el centro de la superficie
 * hacia los bordes, y va desde el color `color1` en el centro hasta el color `color2`
 * en los bordes.
 *
 * @param surface Puntero a la superficie SDL que será llenada con el gradiente radial.
 * @param color1 Color del centro del gradiente (en formato ARGB).
 * @param color2 Color de los bordes del gradiente (en formato ARGB).
 */
void fill_radial_gradient(SDL_Surface *surface, Uint32 color1, Uint32 color2);


/**
 * @brief Carga una textura desde un buffer en memoria.
 *
 * Esta función permite cargar una imagen o textura directamente desde un buffer en memoria.
 * El buffer debe contener los datos de la imagen en un formato compatible con SDL (por ejemplo,
 * un archivo de imagen como PNG o JPEG en formato binario). La función intentará decodificar
 * el contenido del buffer para cargar la textura en una superficie SDL.
 *
 * @param buffer Puntero al buffer de memoria que contiene los datos de la imagen.
 * @param len Longitud del buffer de memoria, es decir, el tamaño en bytes de los datos de la imagen.
 * @return Un puntero a la superficie SDL con la imagen cargada, o NULL en caso de error.
 */
SDL_Surface *Load_Textura_From_Mem( u8 *buffer, int len);

/**
 * @brief Inicializa una superficie para rotarla y escalarla.
 *
 * Esta función prepara una superficie para ser rotada y escalada mediante un ángulo de rotación
 * y un factor de zoom. La superficie original se transforma y se devuelve una nueva superficie
 * con la rotación y el zoom aplicados.
 *
 * @param src Superficie SDL a la que se le aplicará la rotación y el zoom.
 * @param angle Ángulo de rotación en grados (por ejemplo, 90.0 para una rotación de 90 grados).
 * @param zoom Factor de zoom (1.0 para tamaño original, < 1.0 para reducción y > 1.0 para aumento).
 * @return Una nueva superficie SDL con la rotación y el zoom aplicados, o NULL en caso de error.
 */
SDL_Surface *rotozoom_init(SDL_Surface *src, double angle, double zoom);


/**
 * @brief Establece la posición de una superficie rotada y escalada en las coordenadas especificadas.
 *
 * Esta función posiciona la superficie transformada (con rotación y zoom) en las coordenadas (x, y)
 * especificadas. Asegúrate de que la superficie haya sido correctamente inicializada con `rotozoom_init`
 * antes de llamar a esta función.
 *
 * @param src Superficie SDL que fue transformada con rotación y zoom.
 * @param x Coordenada X donde se debe colocar la superficie transformada.
 * @param y Coordenada Y donde se debe colocar la superficie transformada.
 */
void set_surface_rotozoom(SDL_Surface *src, int x, int y);


/**
 * @brief Libera los recursos asociados a la superficie rotada y escalada.
 *
 * Esta función debe llamarse después de usar la superficie rotada y escalada para liberar la memoria
 * utilizada por la superficie transformada. Esto evitará fugas de memoria en el programa.
 *
 * @param src Superficie SDL que fue transformada por `rotozoom_init`.
 */
void rotozoom_finish(SDL_Surface *src);

void set_color(Uint32 color, SDL_PixelFormat * format, Uint8 * r, Uint8 * g, Uint8 * b);

SDL_Surface *cut_surface(SDL_Surface *src, int x, int y, int w, int h);











#ifdef __cplusplus
}
#endif




#endif