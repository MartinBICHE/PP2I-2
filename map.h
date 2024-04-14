#ifndef MAP_H
#define MAP_H

Map *initMap(char*);
float updateCam(float, float);
void loadBackgroundTextures(SDL_Renderer*,SDL_Texture*[],int);
int drawBackground(SDL_Renderer*, SDL_Texture*[], int);
void display_tile(SDL_Renderer*, int, int, int, int , char*, float);
int drawMap(SDL_Renderer*, Map*, char*, float);

#endif
