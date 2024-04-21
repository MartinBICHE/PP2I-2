#ifndef dialog_box_h
#define dialog_box_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "const.h"

void render_box(SDL_Renderer *renderer, SDL_Texture *boxTexture, SDL_Texture *textTexture, int x, int y);



#endif
