#ifndef MAP_H
#define MAP_H
#include <SDL2/SDL.h>
#include "const.h"

/* SDL_Texture *bgTextures[6]; */

Map *init_map(char*) ;
void loadBackgroundTextures(SDL_Renderer*,SDL_Texture*[]);
void display_tile(SDL_Renderer*, int, int, int, int , char*);
int draw_map(SDL_Renderer*, Map*, char*) ;

#endif
