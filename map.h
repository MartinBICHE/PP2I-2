#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>
#include "const.h"

Map *initMap(char*);
void updateCam(Perso*, Map*);
void loadBackgroundTextures(SDL_Renderer*,SDL_Texture*[],int);
int drawBackground(SDL_Renderer*, SDL_Texture*[], int, Map*);
void loadTileTextures(SDL_Renderer*, SDL_Texture**, char*);
void display_tile(SDL_Renderer*, int, int, int, int, Map*, SDL_Texture*);
int drawMap(SDL_Renderer*, Map*, char*, SDL_Texture*);
void destroyMap(Map*);

#endif
