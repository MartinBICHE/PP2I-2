#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "const.h"
#include "enemy1.h"

/* s'utilise avec : */
/* enemy1_movement(renderer, textureEnnemy1, &enemyState) */
/* et */ 
/* EnemyStateData enemyState; */
/* initEnemy1(xPos(à définir), yPost(à définir), &enemyState); */



void enemy1_movement(SDL_Renderer *renderer, SDL_Texture *texture, EnemyStateData *enemyStateData){
    int speed = 64;
    int interval = 1000;
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks/500) % 10;
    switch(enemyStateData->state){
        case MOVING_UP:
            if (SDL_GetTicks() - enemyStateData->pauseStartBits >= interval){
                if (enemyStateData->dst_rect.h <= 128){
                    enemyStateData->src_rect.h += speed;
                    enemyStateData->src_rect.y -= speed;
                    enemyStateData->dst_rect.h += speed;
                    enemyStateData->dst_rect.y -= speed;
                    enemyStateData->pauseStartBits = SDL_GetTicks();
                }else{
                    enemyStateData->state = ANIMATION_START;
                }
            }
                break;
        case ANIMATION_START:
            enemyStateData->src_rect.x += 64;
            /* *i = (*i+1); */
            enemyStateData->src_rect.x = sprite * 64;

            if (sprite == 9){
                enemyStateData->state = PAUSE_TOP;
                enemyStateData->pauseStart = SDL_GetTicks();
            }
            break;
        case PAUSE_TOP:
            if (SDL_GetTicks() - enemyStateData->pauseStart >= interval){
                enemyStateData->state = MOVING_DOWN;
            }
            break;
        case MOVING_DOWN:
            if (SDL_GetTicks() - enemyStateData->pauseStartBits >= interval){
                if (enemyStateData->dst_rect.h >= 2*64){
                    enemyStateData->src_rect.h -= speed;
                    enemyStateData->src_rect.y += speed;
                    enemyStateData->dst_rect.h -= speed;
                    enemyStateData->dst_rect.y += speed;
                    enemyStateData->pauseStartBits = SDL_GetTicks();
                }else{
                    enemyStateData->state = PAUSE_BOTTOM;
                    enemyStateData->pauseStart = SDL_GetTicks();
                }
            }
                break;
            case PAUSE_BOTTOM:
                if (SDL_GetTicks() - enemyStateData->pauseStart >= interval){
                    enemyStateData->state = MOVING_UP;
                }
                break;
    } 
    SDL_RenderCopy(renderer, texture, &enemyStateData->src_rect, &enemyStateData->dst_rect);

}


void initEnemy1(int x, int y, EnemyStateData *enemyStateData){
    enemyStateData->src_rect.x = 0;
    enemyStateData->src_rect.y = 128;
    enemyStateData->src_rect.w = 64;
    enemyStateData->src_rect.h = 64;
    enemyStateData->dst_rect.x = x;
    enemyStateData->dst_rect.y = y;
    enemyStateData->dst_rect.w = 64;
    enemyStateData->dst_rect.h = 64;
    enemyStateData->pauseStart = 0;
    enemyStateData->pauseStartBits = 0;
}






