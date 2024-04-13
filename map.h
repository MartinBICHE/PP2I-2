#ifndef MAP_H
#define MAP_H

Map *init_map(char*) ;
void loadBackgroundTextures(SDL_Renderer*,SDL_Texture*[],int);
void display_tile(SDL_Renderer*, int, int, int, int , char*);
int draw_map(SDL_Renderer*, Map*, char*) ;

#endif
