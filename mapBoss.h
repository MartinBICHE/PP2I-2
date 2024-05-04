#ifndef MAPBOSS_H
#define MAPBOSS_H
#include <SDL2/SDL.h>
#include "const.h"

Map *initMapBoss(char*);
void updateCamBoss(Perso*,Map*);
void loadBackgroundTexturesBoss(SDL_Renderer*,SDL_Texture*[],int);
int drawBackgroundBoss(SDL_Renderer*, SDL_Texture*[], int, Map*);
void loadTileTexturesBoss(SDL_Renderer*, SDL_Texture**, char*);
void display_tileBoss(SDL_Renderer*, int, int, int, int,Map*, SDL_Texture*);
int drawMapBoss(SDL_Renderer*, Map*, char*, SDL_Texture*);
void destroyMapBoss(Map*);

#endif