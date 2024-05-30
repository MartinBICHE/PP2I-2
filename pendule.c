#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "pendule.h"
#include <math.h>
#include "textures.h"


void enemyPendule_mouvement(SDL_Renderer *renderer, EnemyPenduleData *enemyPenduleData, Map *map) {
  int interval = 100;
  int pauseInterval = 3000;

  switch (enemyPenduleData->state) {
  case PENDULE_MOVING_UP:
    if (SDL_GetTicks() - enemyPenduleData->pauseStartBits >= interval) {
      enemyPenduleData->src_rect.x -= 64;
      enemyPenduleData->pauseStartBits = SDL_GetTicks();
    }
    if (enemyPenduleData->src_rect.x == 0) {
      enemyPenduleData->state = PENDULE_PAUSE_TOP;
      enemyPenduleData->pauseStart = SDL_GetTicks();
    }
    break;
  case PENDULE_PAUSE_TOP:
    if (SDL_GetTicks()-enemyPenduleData->pauseStart >= pauseInterval){
        enemyPenduleData->state = PENDULE_MOVING_DOWN;
    }
    break;
  case PENDULE_PAUSE_BOTTOM:
    if (SDL_GetTicks() - enemyPenduleData->pauseStart >= pauseInterval){
        enemyPenduleData->state = PENDULE_MOVING_UP;
    }
    break;
  case PENDULE_MOVING_DOWN:
    if (SDL_GetTicks() - enemyPenduleData->pauseStartBits >= interval) {
      enemyPenduleData->src_rect.x += 64;
      /* enemyPenduleData->dst_rect.h += 64; */
      enemyPenduleData->pauseStartBits = SDL_GetTicks();
    }
    if (enemyPenduleData->src_rect.x == 6 * 64) {
      enemyPenduleData->state = PENDULE_PAUSE_BOTTOM;
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
    int margin = 40;
    enemyHitbox.w -= 3*margin;
    enemyHitbox.h -= 2*margin;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}

void penduleAttack(EnemyPenduleData *enemy, Perso *perso, Map *map){
    int intervalAttack = 1000;
    if (hitbox_enemyPendule(perso, map, enemy)){
        if (SDL_GetTicks() - enemy->pauseAttack >= intervalAttack && perso->health >= 0 ){
            if (enemy->state != PENDULE_PAUSE_BOTTOM && enemy->state != PENDULE_PAUSE_TOP){
                perso->health -= 1;
                enemy->pauseAttack = SDL_GetTicks();
            }
        }
    }
}

void updatePersoPendule(Perso *perso, Map *map, EnemyPenduleData *enemy){
    if (!isBossMap){
        if (hitbox_enemyPendule(perso, map, enemy)){
            if (enemy->state != PENDULE_PAUSE_BOTTOM && enemy->state != PENDULE_PAUSE_TOP){
            float dx = perso->vx * DT;
            float dy = perso->vy * DT;
            if (dx > 0) { // Le personnage se déplace vers la droite
                perso->vx = max(perso->vx, 0.0f);
                // Position juste avant le début de la hitbox de l'ennemi (côté gauche)
                perso->x = enemy->dst_rect.x / map->pix_rect - PERSO_WIDTH / 2.0f + 0.5;
            } else if (dx < 0) { // Le personnage se déplace vers la gauche
                perso->vx = min(perso->vx, 0.0f);
                // Position juste avant le début de la hitbox de l'ennemi (côté droit)
                perso->x = (enemy->dst_rect.x + enemy->dst_rect.w) / map->pix_rect + PERSO_WIDTH / 2.0f + 0.3;
            }
            if (dy > 0) { // Le personnage se déplace vers le bas
                // Faire rebondir le personnage au dessus de l'ennemi
                perso->vy = -JUMPSPEED;
        }
        }

            }
    }
}

