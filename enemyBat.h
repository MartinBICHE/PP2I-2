#ifndef enemyBat_h
#define enemyBat_h
#include "enemyFleche.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

struct _EnemyBatData{
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseStartBits;
};
typedef struct _EnemyBatData EnemyBatData;
void enemyBat_mouvement(SDL_Renderer *renderer, SDL_Texture *texture,
                        EnemyBatData *enemyBatData);
void initEnemyBat(EnemyBatData *enemyBatData, int x, int y);

#endif
