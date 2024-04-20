#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "const.h"
#include <SDL2/SDL_ttf.h>

void scroll_movement(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src_rect, SDL_Rect *dst_rect, TTF_Font *font, const char *text, SDL_Color color);
