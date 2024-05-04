#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "enemyFleche.h"
#include "textures.h"

    /* enemyFleche_mouvement(renderer, &enemyFlecheData); */
  /* EnemyFlecheData enemyFlecheData; */
  /* initEnemyFleche(&enemyFlecheData, x(à définir), y(à définir)); */

void enemyFleche_mouvement(SDL_Renderer *renderer, EnemyFlecheData *enemyFlecheData){
    int interval = 400;
    int pauseInterval = 2000;

    switch(enemyFlecheData->state){
        case FLECHE_MOVING_UP:
            if (SDL_GetTicks() - enemyFlecheData->pauseStartBits >= interval){
                enemyFlecheData->src_rect.x -= 64;
                /* enemyFlecheData->dst_rect.h -= 64; */
                enemyFlecheData->pauseStartBits = SDL_GetTicks();
            }
            if (enemyFlecheData->src_rect.x == 0){
                enemyFlecheData->state = FLECHE_PAUSE_TOP;
                enemyFlecheData->pauseStart = SDL_GetTicks();
            }
            break;
        case FLECHE_PAUSE_TOP:
            if (SDL_GetTicks() - enemyFlecheData->pauseStart >= pauseInterval){
                enemyFlecheData->state = FLECHE_MOVING_DOWN;
            }
            break;
        case FLECHE_PAUSE_BOTTOM:
            if (SDL_GetTicks() - enemyFlecheData->pauseStart >= pauseInterval){
                enemyFlecheData->state = FLECHE_MOVING_UP;
            }
            break;
        case FLECHE_MOVING_DOWN:
            if (SDL_GetTicks() - enemyFlecheData->pauseStartBits >= interval){
                enemyFlecheData->src_rect.x += 64;
                /* enemyFlecheData->dst_rect.h += 64; */
                enemyFlecheData->pauseStartBits = SDL_GetTicks();
            }
            if (enemyFlecheData->src_rect.x == 4*64){
                enemyFlecheData->state = FLECHE_PAUSE_BOTTOM;
                enemyFlecheData->pauseStart = SDL_GetTicks();
            }
            break;

    }
    SDL_RenderCopy(renderer, textureFleche, &enemyFlecheData->src_rect, &enemyFlecheData->dst_rect);
}

void initEnemyFleche(EnemyFlecheData *enemyFlecheData, int x, int y){
    enemyFlecheData->src_rect.x = 4*64;
    enemyFlecheData->src_rect.y = 0;
    enemyFlecheData->src_rect.w = 64;
    enemyFlecheData->src_rect.h = 64;

    enemyFlecheData->dst_rect.x = x;
    enemyFlecheData->dst_rect.y = y;
    enemyFlecheData->dst_rect.w = 64*3;
    enemyFlecheData->dst_rect.h = 64*3;

    enemyFlecheData->state = FLECHE_MOVING_UP;
    enemyFlecheData->pauseStartBits = 0; 
}