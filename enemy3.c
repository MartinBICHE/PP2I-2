#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "enemy3.h"

/* s'utilise avec : */
/*     enemy3_movement(renderer, textureEnnemy3, &enemy); */
/*     Enemy3 enemy; */
/*     initEnemy3(&enemy, xPos(à définir), yPos(à définir), xCollisionMax(à définir)) */

void enemy3_movement(SDL_Renderer *renderer, SDL_Texture *texture, Enemy3 *enemy){
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 400) % 9;

    enemy->xPosition += enemy->dx * enemy->speed * 0.1;
    if (enemy->xPosition <=  enemy->xCollisionMin || enemy->xPosition >= enemy->xCollisionMax ) {
        enemy->dx *= -1;
    }
    enemy->src_rect.x = sprite * 64;
    enemy->dst_rect.x += enemy->dx * enemy->speed * 0.1;
    if (enemy->dx == 1){
        SDL_RenderCopyEx(renderer, texture, &enemy->src_rect, &enemy->dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (enemy->dx == -1){
        SDL_RenderCopy(renderer, texture, &enemy->src_rect, &enemy->dst_rect);
    }
}

void initEnemy3(Enemy3 *enemy, int x, int y, int xCollisionMax){
    enemy->src_rect.x = 0;
    enemy->src_rect.y = 0;
    enemy->src_rect.w = 64;
    enemy->src_rect.h = 64;

    enemy->dst_rect.x = x;
    enemy->dst_rect.y = y;
    enemy->dst_rect.w = 64;
    enemy->dst_rect.h = 64;

    enemy->xPosition = enemy->dst_rect.x;
    enemy->yPosition = enemy->dst_rect.y;
    enemy->speed = 10;
    enemy->xCollisionMax = xCollisionMax;
    enemy->xCollisionMin = enemy->dst_rect.x;
    enemy->dx = 1;


}


