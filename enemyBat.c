#include "enemyBat.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

/* s'utilise avec: */
/*   EnemyBatData enemyBatData; */
/*   initEnemyBat(&enemyBatData, xPos(à définir), yPos(à définir), xMax(à définir)); */
/* enemyBat_mouvement(renderer, &enemyBatData); */

void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData) {
    int interval = 130;
    int speed = 20;
    if (enemyBatData->state == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &enemyBatData->dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (enemyBatData->state == BAT_MOVING_LEFT){
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &enemyBatData->dst_rect);
    }

    switch(enemyBatData->state){
        case BAT_MOVING_RIGHT:
            if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
                enemyBatData->dst_rect.x += speed;
                enemyBatData->src_rect.x += 32;
                enemyBatData->pauseStartBits = SDL_GetTicks();
            }
            if (enemyBatData->src_rect.x == 160){
                enemyBatData->src_rect.x = 0;
            }
            if (enemyBatData->dst_rect.x == enemyBatData->xMax){
                enemyBatData->state = BAT_MOVING_LEFT;
            }
        case BAT_MOVING_LEFT:
        if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
            enemyBatData->dst_rect.x -= speed;
            enemyBatData->src_rect.x += 32;
            enemyBatData->pauseStartBits = SDL_GetTicks();
        }
        if (enemyBatData->src_rect.x == 160){
            enemyBatData->src_rect.x = 0;
        }
        if (enemyBatData->dst_rect.x == enemyBatData->xMin){
            enemyBatData->state = BAT_MOVING_RIGHT;
        }
    }
}


void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax){

    enemyBatData->src_rect.x = 0;
    enemyBatData->src_rect.y = 0;
    enemyBatData->src_rect.w = 32;
    enemyBatData->src_rect.h = 64;

    enemyBatData->dst_rect.x = x;
    enemyBatData->dst_rect.y = y;
    enemyBatData->dst_rect.w = 32*3;
    enemyBatData->dst_rect.h = 64*3;

    enemyBatData->pauseStartBits = 0;
    enemyBatData->state = BAT_MOVING_RIGHT;
    enemyBatData->xMax = xMax;
    enemyBatData->xMin = x;
}