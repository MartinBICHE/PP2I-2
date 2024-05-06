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
    if (enemyBatData->state == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (enemyBatData->state == BAT_MOVING_LEFT){
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rect);
    }
    if (enemyBatData->state == BAT_ATTACK){
        SDL_RenderCopy(renderer, textureEnemy1, &enemyBatData->src_rect, &dst_rect);
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
        case BAT_MOVING_LEFT:
        if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
            enemyBatData->dst_rect.x -= speed;
            enemyBatData->src_rect.x += 32;
            enemyBatData->pauseStartBits = SDL_GetTicks();
        }
        if (enemyBatData->src_rect.x == 160){
            enemyBatData->src_rect.x = 0;
        }
        if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMin) <= position_tolerance){
            enemyBatData->state = BAT_MOVING_RIGHT;
        }
        case BAT_ATTACK:
        if (SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack){
            enemyBatData->state = BAT_MOVING_LEFT;
        }
        /* à resoudre pb lorsqu'il ne bouge plus dans l'autre direction */
        /* et le problème de musique pour tous les trucs */
        /* et les hitbox pour chaque personnage */

        break;
    }

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
    enemyBatData->pauseAttack = 0;
    enemyBatData->state = BAT_MOVING_RIGHT;
    enemyBatData->xMax = xMax;
    enemyBatData->xMin = x;
}


void batAttack(EnemyBatData *enemyBatData, Perso *perso, Map *map){
    int channelAttack = -1;
    int intervalAttack = 1000;
    int spriteLength = 64;
    int channelWings = -1;
    float borneInf = enemyBatData->dst_rect.x;
    float borneSup = borneInf + spriteLength;
    float persoPositionX = perso->x * map->pix_rect;
    /* printf("borneInf %f\n", borneInf); */
    /* printf("persoPositionX %f\n", persoPositionX); */
    if (persoPositionX >= borneInf && persoPositionX <= borneSup){
        if (SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack){
            perso->health -= 1;
            enemyBatData->pauseAttack = SDL_GetTicks();
            channelAttack = Mix_PlayChannel(-1, musicEnemyBatAttack, 0);
            enemyBatData->pauseMusic = SDL_GetTicks();
            enemyBatData->state = BAT_ATTACK;
        }
    }
    if (SDL_GetTicks() - enemyBatData->pauseMusic >= intervalAttack){
        Mix_HaltChannel(channelAttack);
        enemyBatData->pauseMusic = 0;
    }

}
