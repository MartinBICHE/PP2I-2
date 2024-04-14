#ifndef MC_H
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include "const.h"
#define MC_H

int spriteMovement(SDL_Renderer*, SDL_Event, int*, int*, const char*, Map *map);

#endif
