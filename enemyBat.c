#include "enemyBat.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

void enemyBat_mouvement(SDL_Renderer *renderer, SDL_Texture *texture,
                        EnemyBatData *enemyBatData) {
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 100) % 6;
  /* int interval = 1000; */
    enemyBatData->src_rect.x = sprite * 32;
    enemyBatData->dst_rect.x += 2 ;

    SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &enemyBatData->dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
}


void initEnemyBat(EnemyBatData *enemyBatData, int x, int y){

    enemyBatData->src_rect.x = 0;
    enemyBatData->src_rect.y = 0;
    enemyBatData->src_rect.w = 32;
    enemyBatData->src_rect.h = 64;

    enemyBatData->dst_rect.x = x;
    enemyBatData->dst_rect.y = y;
    enemyBatData->dst_rect.w = 32*3;
    enemyBatData->dst_rect.h = 64*3;

    enemyBatData->pauseStartBits = 0;
}
