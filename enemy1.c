#include "enemy1.h"
#include "const.h"
#include "main.h"
#include "music.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include "perso.h"

/* s'utilise avec : */
/* enemy1_movement(renderer, &enemyState, x_cam) */
/* et */
/* EnemyStateData enemyState; */
/* initEnemy1(xPos(à définir), yPost(à définir), &enemyState); */

/* pour l'attaque; */
/* enemy1Attack(&enemyStateData, perso, map); */
/* renderStatusHealth(renderer, perso); pour voir l'impact sur la santé */

void enemy1_movement(SDL_Renderer *renderer, EnemyStateData *enemyStateData,
                     Map *map) {
  int speed = 64;
  int interval = 500;
  int pauseInterval = 3000;
  int pauseIntervalTop = 1000;
  Uint32 ticks = SDL_GetTicks();
  Uint32 sprite = (ticks / 500) % 10;
  switch (enemyStateData->state) {
  case MOVING_UP:
    if (SDL_GetTicks() - enemyStateData->pauseStartBits >= interval) {
      if (enemyStateData->dst_rect.h <= 128) {
        enemyStateData->src_rect.h += speed;
        enemyStateData->src_rect.y -= speed;
        enemyStateData->dst_rect.h += speed;
        enemyStateData->dst_rect.y -= speed;
        enemyStateData->pauseStartBits = SDL_GetTicks();

      } else {
        
        enemyStateData->state = ANIMATION_START;
      }
    }
    break;
  case ANIMATION_START:
    enemyStateData->src_rect.x += 64;
    enemyStateData->src_rect.x = sprite * 64;

    if (sprite == 9) {
      enemyStateData->state = PAUSE_TOP;
      enemyStateData->pauseStart = SDL_GetTicks();
    }
    break;
  case PAUSE_TOP:
    if (SDL_GetTicks() - enemyStateData->pauseStart >= pauseIntervalTop) {
      enemyStateData->state = MOVING_DOWN;
    }
    break;
  case MOVING_DOWN:
    if (SDL_GetTicks() - enemyStateData->pauseStartBits >= interval) {
      if (enemyStateData->dst_rect.h >= 2 * 64) {
        enemyStateData->src_rect.h -= speed;
        enemyStateData->src_rect.y += speed;
        enemyStateData->dst_rect.h -= speed;
        enemyStateData->dst_rect.y += speed;
        enemyStateData->pauseStartBits = SDL_GetTicks();
      } else {
        enemyStateData->state = PAUSE_BOTTOM;
        enemyStateData->pauseStart = SDL_GetTicks();
      }
    }
    break;
  case PAUSE_BOTTOM:
    if (SDL_GetTicks() - enemyStateData->pauseStart >= pauseInterval) {
      enemyStateData->state = MOVING_UP;
      enemyStateData->pauseStart = SDL_GetTicks();
    }
    break;
  }
  SDL_Rect dst_rectFixed = {
      enemyStateData->dst_rect.x - map->x_cam, enemyStateData->dst_rect.y,
      enemyStateData->dst_rect.w, enemyStateData->dst_rect.h};
  SDL_RenderCopy(renderer, textureEnemy1, &enemyStateData->src_rect,
                 &dst_rectFixed);
}

void initEnemy1(int x, int y, EnemyStateData *enemyStateData) {
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

void enemy1Attack(EnemyStateData *enemyStateData, Perso *perso, Map *map) {
  int intervalAttack = 1000;
  if (hitbox_enemy1(perso, map, enemyStateData)){
      if (SDL_GetTicks() - enemyStateData->pauseAttack >= intervalAttack){
          if (enemyStateData->state != PAUSE_BOTTOM){
              if (perso->health > 0){
                  perso->health -= 1;
                  enemyStateData->pauseAttack = SDL_GetTicks();
                  /* à mettre effet sonore et animation */
              }
          }
      }
  }
}

int hitbox_enemy1(Perso *perso, Map *map, EnemyStateData *enemyStateData) {
    SDL_Rect enemyHitbox = enemyStateData->dst_rect;
    int margin = 10; // Marge pour que le personnage ne soit pas collé à la hitbox de l'ennemi
    enemyHitbox.x -= margin;
    enemyHitbox.y -= margin;
    enemyHitbox.w += 2 * margin;
    enemyHitbox.h += 2 * margin;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}

void updatePersoEnemy1(Perso *perso, Map *map, EnemyStateData *enemyStateData){
    if (!isBossMap){
        if (enemyStateData->state != PAUSE_BOTTOM){
            if (hitbox_enemy1(perso, map, enemyStateData)){
                float dx = perso->vx * DT;
                float dy = perso->vy * DT;
                if (dx > 0) { // Le personnage se déplace vers la droite
                    perso->vx = max(perso->vx, 0.0f);
                    // Position juste avant le début de la hitbox de l'ennemi (côté gauche)
                    perso->x = enemyStateData->dst_rect.x / map->pix_rect - PERSO_WIDTH / 2.0f + 0.5;
                } else if (dx < 0) { // Le personnage se déplace vers la gauche
                    perso->vx = min(perso->vx, 0.0f);
                    // Position juste avant le début de la hitbox de l'ennemi (côté droit)
                    perso->x = (enemyStateData->dst_rect.x + enemyStateData->dst_rect.w) / map->pix_rect + PERSO_WIDTH / 2.0f + 0.3;
                }
                if (dy > 0) { // Le personnage se déplace vers le bas
                    // Faire rebondir le personnage au dessus de l'ennemi
                    perso->vy = -JUMPSPEED;
                }
            }
        }
    }
}
