#ifndef FIGHT_H
#define FIGHT_H
#include <SDL2/SDL.h>
#include "const.h"

int showRectangle(SDL_Renderer*, int, int, int, int);
int fightMovement(SDL_Renderer*, SDL_Event, Perso*);

#endif
