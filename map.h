#ifndef MAP_H
#define MAP_H

SDL_Texture *bgTextures[6];

Map *init_map(char*) ;
void loadBackgroundTextures(SDL_Renderer*,SDL_Texture*);
void display_tile(SDL_Renderer*, int, int, int, int , char*);
int draw_map(SDL_Renderer*, Map*, char*) ;
void closeMapWindow(SDL_Window *window, SDL_Renderer *renderer) ;

#endif
