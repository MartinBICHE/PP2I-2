#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>
#include "const.h"

Map *initMap(char*);
void updateCam(Perso*, Map*);
void loadBackgroundTextures(SDL_Renderer*,SDL_Texture*[],int);
int drawBackground(SDL_Renderer*, SDL_Texture*[], int, Map*);
void display_tile(SDL_Renderer*, int, int, int, int , char*, float);
int drawMap(SDL_Renderer*, Map*, char*, float);

#endif
