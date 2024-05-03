#ifndef MAPBOSS_H
#define MAPBOSS_H
#include <SDL2/SDL.h>
#include "const.h"

Map *initMapBoss(char*);
float updateCamBoss(float, float);
void loadBackgroundTexturesBoss(SDL_Renderer*,SDL_Texture*[],int);
int drawBackgroundBoss(SDL_Renderer*, SDL_Texture*[], int, float);
void loadTileTexturesBoss(SDL_Renderer*, SDL_Texture**, char*);
void display_tileBoss(SDL_Renderer*, int, int, int, int, float, SDL_Texture*);
int drawMapBoss(SDL_Renderer*, Map*, char*, float, SDL_Texture*);

#endif