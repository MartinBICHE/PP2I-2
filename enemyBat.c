#include "enemyBat.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include "music.h"

/* s'utilise avec: */
  /* EnemyBatData enemyBatData; */
/*   initEnemyBat(&enemyBatData, xPos(à définir), yPos(à définir), xMax(à définir)); */
/* enemyBat_mouvement(renderer, &enemyBatData, map); */

void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Map *map) {
    int interval = 130;
    int intervalAttack = 1000;
    int speed = 20;
    double position_tolerance = 20;
    SDL_Rect dst_rect = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    SDL_Rect dst_rect2 = {enemyBatData->dst_rectAttack.x - map->x_cam, enemyBatData->dst_rectAttack.y, 64*2, enemyBatData->dst_rectAttack.h};
    SDL_Rect dst_rect2Ex = {enemyBatData->dst_rectAttack.x - map->x_cam - 25, enemyBatData->dst_rectAttack.y, 64*2, enemyBatData->dst_rectAttack.h};
    int pauseInterval = 1000;
    if (enemyBatData->state == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (enemyBatData->state == BAT_MOVING_LEFT){
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rect);
    }
    if (enemyBatData->state == BAT_ATTACK){
        if (enemyBatData->previousState == BAT_MOVING_LEFT){
            SDL_RenderCopy(renderer, textureBatAttack, &enemyBatData->src_rectAttack, &dst_rect2);
        }
        if (enemyBatData->previousState == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBatAttack, &enemyBatData->src_rectAttack, &dst_rect2Ex, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
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
            if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMax) <= position_tolerance){
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            break;
        case BAT_MOVING_LEFT:
        if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMin) <= position_tolerance){
            enemyBatData->state = BAT_MOVING_RIGHT;
        }
        if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
            enemyBatData->dst_rect.x -= speed;
            enemyBatData->src_rect.x += 32;
            enemyBatData->pauseStartBits = SDL_GetTicks();
        }
        if (enemyBatData->src_rect.x == 160){
            enemyBatData->src_rect.x = 0;
        }
        break;
        case BAT_ATTACK:
        enemyBatData->dst_rectAttack.x = enemyBatData->dst_rect.x;
            if (SDL_GetTicks() - enemyBatData->pauseStartBitsAttack >= interval){
                enemyBatData->src_rectAttack.x += 1*64;
                enemyBatData->pauseStartBitsAttack = SDL_GetTicks();
            }
            printf("x %d\n", enemyBatData->src_rectAttack.x);
            if (fabs((double)enemyBatData->src_rectAttack.x - 448) <= position_tolerance){
                enemyBatData->state = enemyBatData->previousState;
                enemyBatData->src_rectAttack.x = 0;
            }

        /* et le problème de musique pour tous les trucs */
        /* et les hitbox pour chaque personnage */

            break;
    }
    printf("state: %d\n", enemyBatData->state);

}


void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax){

    enemyBatData->src_rect.x = 0;
    enemyBatData->src_rect.y = 0;
    enemyBatData->src_rect.w = 32;
    enemyBatData->src_rect.h = 64;

    enemyBatData->dst_rect.x = x;
    enemyBatData->dst_rect.y = y;
    enemyBatData->dst_rect.w = 32*2;
    enemyBatData->dst_rect.h = 64*2;

    enemyBatData->pauseStartBits = 0;
    enemyBatData->pauseStartBitsAttack = 0;
    enemyBatData->pauseAttack = 0;
    enemyBatData->state = BAT_MOVING_RIGHT;
    enemyBatData->xMax = xMax;
    enemyBatData->xMin = x;

    enemyBatData->src_rectAttack.x = 0; 
    enemyBatData->src_rectAttack.y = 0;
    enemyBatData->src_rectAttack.w = 64;
    enemyBatData->src_rectAttack.h = 64*2;

    enemyBatData->dst_rectAttack.x = x;
    enemyBatData->dst_rectAttack.y = y;
    enemyBatData->dst_rectAttack.w = 64*2;
    enemyBatData->dst_rectAttack.h = 64*2;
}


void batAttack(EnemyBatData *enemyBatData, Perso *perso, Map *map){
    int intervalAttack = 1000;
    int spriteLength = 64;
    float borneInf = enemyBatData->dst_rect.x;
    float borneSup = borneInf + spriteLength;
    float persoPositionX = perso->x * map->pix_rect;

    if (enemyBatData->state != BAT_ATTACK){
        enemyBatData->previousState = enemyBatData->state;
    }

    if (persoPositionX >= borneInf && persoPositionX <= borneSup){
        if (SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack){
            perso->health -= 1;
            enemyBatData->pauseAttack = SDL_GetTicks();
            enemyBatData->pauseMusic = SDL_GetTicks();
            enemyBatData->state = BAT_ATTACK;
        }
    }
    /* if (SDL_GetTicks() - enemyBatData->pauseMusic >= intervalAttack){ */
    /*     Mix_HaltChannel(channelAttack); */
    /*     enemyBatData->pauseMusic = 0; */
    /* } */


       if (enemyBatData->state == BAT_ATTACK && SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack) {
        enemyBatData->state = enemyBatData->previousState;
        enemyBatData->pauseAttack = SDL_GetTicks();
    }

}
