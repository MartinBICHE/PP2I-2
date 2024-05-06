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
#include "textures.h"
#include "main.h"
#include "enemy1.h"
#include "music.h"

/* s'utilise avec : */
/* enemy1_movement(renderer, &enemyState, x_cam) */
/* et */ 
/* EnemyStateData enemyState; */
/* initEnemy1(xPos(à définir), yPost(à définir), &enemyState); */


/* pour l'attaque; */
/* enemy1Attack(&enemyStateData, perso, map); */
/* renderStatusHealth(renderer, perso); pour voir l'impact sur la santé */


void enemy1_movement(SDL_Renderer *renderer, EnemyStateData *enemyStateData, Map *map) {
    int speed = 64;
    int interval = 500;
    int pauseInterval = 3000;
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
                if (SDL_GetTicks() - enemyStateData->pauseStart >= pauseInterval){
                    enemyStateData->state = MOVING_UP;
                }
                break;
    } 
    SDL_Rect dst_rectFixed = {
        enemyStateData->dst_rect.x - map->x_cam, enemyStateData->dst_rect.y, enemyStateData->dst_rect.w, enemyStateData->dst_rect.h};
    SDL_RenderCopy(renderer, textureEnemy1, &enemyStateData->src_rect, &dst_rectFixed);
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
    enemyStateData->pauseAttack = 0;
    enemyStateData->pauseMusic = 0;

}


void enemy1Attack(EnemyStateData *enemyStateData, Perso *perso, Map *map){
    int intervalAttack = 1000;
    int pad = 50;
    /* Mix_VolumeMusic(MIX_MAX_VOLUME / 3); */
    int spriteLength = 64;
    float borneInf = enemyStateData->dst_rect.x;
    float borneSup = borneInf + spriteLength;
    float persoPositionX = perso->x * map->pix_rect;

    if (persoPositionX >= borneInf && persoPositionX <= borneSup){
        if (enemyStateData->state != PAUSE_BOTTOM && perso->health > 0){
            if (SDL_GetTicks() - enemyStateData->pauseAttack >= intervalAttack){
                perso->health -= 1;
                enemyStateData->pauseAttack = SDL_GetTicks();
                Mix_PlayMusic(musicEnemyFleche, -1);
            }
        }
    }
    if (SDL_GetTicks() - enemyStateData->pauseMusic >= intervalAttack){
        Mix_HaltMusic();
        enemyStateData->pauseMusic = SDL_GetTicks();
    }
}
