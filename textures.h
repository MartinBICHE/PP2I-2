#ifndef TEXTURES_H
#define TEXTURES_H
#include <SDL2/SDL.h>

extern SDL_Texture *textureEnemy1;
extern SDL_Texture *textureEnemy3;
extern SDL_Texture *texturePapirus;
extern SDL_Texture *textureScroll;
extern SDL_Texture *texturePendule;
extern SDL_Texture* textureHealth0;
extern SDL_Texture* textureHealth1;
extern SDL_Texture* textureHealth2;
extern SDL_Texture* textureHealth3;
extern SDL_Texture* textureHealth4;
extern SDL_Texture* textureHealth5;
extern SDL_Texture* textureHealth6;
extern SDL_Texture* textureHealth7;
extern SDL_Texture* textureHealth8;
extern SDL_Texture* textureHealth9;

extern SDL_Texture* textureFleche;
extern SDL_Texture* textureBat;

extern SDL_Texture *healthTextures[10];

int loadTextures(SDL_Renderer *renderer);

int freeTextures(void);


#endif