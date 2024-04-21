#ifndef enemy1_h
#define enemy1_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void enemy1_movement(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src_rect, SDL_Rect *dst_rect, int *i);

#endif
