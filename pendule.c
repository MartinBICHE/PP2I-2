#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "pendule.h"
#include <math.h>
#include "textures.h"


void enemyPendule_mouvement(SDL_Renderer *renderer, EnemyPenduleData *enemyPenduleData, Map *map) {
  int interval = 200;
  int pauseInterval = 3000;

  switch (enemyPenduleData->state) {
  case PENDULE_MOVING_UP:
    if (SDL_GetTicks() - enemyPenduleData->pauseStartBits >= interval) {
      enemyPenduleData->src_rect.x -= 64;
      enemyPenduleData->pauseStartBits = SDL_GetTicks();
    }
    if (enemyPenduleData->src_rect.x == 0) {
      enemyPenduleData->state = PENDULE_MOVING_DOWN;
      enemyPenduleData->pauseStart = SDL_GetTicks();
    }
    break;
  case PENDULE_MOVING_DOWN:
    if (SDL_GetTicks() - enemyPenduleData->pauseStartBits >= interval) {
      enemyPenduleData->src_rect.x += 64;
      /* enemyPenduleData->dst_rect.h += 64; */
      enemyPenduleData->pauseStartBits = SDL_GetTicks();
    }
    if (enemyPenduleData->src_rect.x == 6 * 64) {
      enemyPenduleData->state = PENDULE_MOVING_UP;
      enemyPenduleData->pauseStart = SDL_GetTicks();
    }
    break;
  }
  SDL_Rect dst_rectFixed = {
      enemyPenduleData->dst_rect.x - map->x_cam, enemyPenduleData->dst_rect.y,
      enemyPenduleData->dst_rect.w, enemyPenduleData->dst_rect.h};
  SDL_RenderCopy(renderer, texturePendule, &enemyPenduleData->src_rect,
                 &dst_rectFixed);
}


void initEnemyPendule(EnemyPenduleData *enemyPenduleData, int x, int y) {
  enemyPenduleData->src_rect.x = 1 * 64;
  enemyPenduleData->src_rect.y = 0;
  enemyPenduleData->src_rect.w = 64;
  enemyPenduleData->src_rect.h = 64;

  enemyPenduleData->dst_rect.x = x;
  enemyPenduleData->dst_rect.y = y;
  enemyPenduleData->dst_rect.w = 64 * 3;
  enemyPenduleData->dst_rect.h = 64 * 3;

  enemyPenduleData->state = PENDULE_MOVING_UP;
  enemyPenduleData->pauseStartBits = 0;
  enemyPenduleData->pauseAttack = 0;
  enemyPenduleData->attackCounter = 0;
  enemyPenduleData->pauseMusic = 0;
}

int hitbox_enemyPendule(Perso *perso, Map *map, EnemyPenduleData *enemy){
    SDL_Rect enemyHitbox = enemy->dst_rect;
    int margin = 10; // Marge pour que le personnage ne soit pas collé à la hitbox de l'ennemi
    enemyHitbox.x -= margin;
    enemyHitbox.y -= margin;
    enemyHitbox.w += margin;
    enemyHitbox.h -= 1*margin;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}

void penduleAttack(EnemyPenduleData *enemy, Perso *perso, Map *map){
    int intervalAttack = 1000;
    if (hitbox_enemyPendule(perso, map, enemy)){
        if (SDL_GetTicks() - enemy->pauseAttack >= intervalAttack && perso->health >= 0){
            perso->health -= 1;
            enemy->pauseAttack = SDL_GetTicks();
        }
    }
}

