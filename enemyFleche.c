#include "enemyFleche.h"
#include "perso.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mixer.h>
#include "music.h"

/* enemyFleche_mouvement(renderer, &enemyFlecheData, map); */
/* EnemyFlecheData enemyFlecheData; */
/* initEnemyFleche(&enemyFlecheData, x(à définir), y(à définir)); */
/* flecheAttack(EnemyFlecheData *enemyFlecheData, Perso *perso, Map *map)  pour l'attaque*/
        /* renderStatusHealth(renderer, perso); pour voir l'impact sur la santé */

void enemyFleche_mouvement(SDL_Renderer *renderer, EnemyFlecheData *enemyFlecheData, Map *map) {
  int interval = 100;
  int pauseInterval = 3000;
  int pauseIntervalTop = 1000;

  switch (enemyFlecheData->state) {
  case FLECHE_MOVING_UP:
    if (SDL_GetTicks() - enemyFlecheData->pauseStartBits >= interval) {
      enemyFlecheData->src_rect.x -= 64;
      enemyFlecheData->pauseStartBits = SDL_GetTicks();
    }
    if (enemyFlecheData->src_rect.x == 0) {
      enemyFlecheData->state = FLECHE_PAUSE_TOP;
      enemyFlecheData->pauseStart = SDL_GetTicks();
    }
    break;
  case FLECHE_PAUSE_TOP:
    if (SDL_GetTicks() - enemyFlecheData->pauseStart >= pauseIntervalTop) {
      enemyFlecheData->state = FLECHE_MOVING_DOWN;
    }
    break;
  case FLECHE_PAUSE_BOTTOM:
    if (SDL_GetTicks() - enemyFlecheData->pauseStart >= pauseInterval) {
      enemyFlecheData->state = FLECHE_MOVING_UP;
    }
    break;
  case FLECHE_MOVING_DOWN:
    if (SDL_GetTicks() - enemyFlecheData->pauseStartBits >= interval) {
      enemyFlecheData->src_rect.x += 64;
      /* enemyFlecheData->dst_rect.h += 64; */
      enemyFlecheData->pauseStartBits = SDL_GetTicks();
    }
    if (enemyFlecheData->src_rect.x == 4 * 64) {
      enemyFlecheData->state = FLECHE_PAUSE_BOTTOM;
      enemyFlecheData->pauseStart = SDL_GetTicks();
    }
    break;
  }
  SDL_Rect dst_rectFixed = {
      enemyFlecheData->dst_rect.x - map->x_cam, enemyFlecheData->dst_rect.y,
      enemyFlecheData->dst_rect.w, enemyFlecheData->dst_rect.h};
  SDL_RenderCopy(renderer, textureFleche, &enemyFlecheData->src_rect,
                 &dst_rectFixed);
}

void initEnemyFleche(EnemyFlecheData *enemyFlecheData, int x, int y) {
  enemyFlecheData->src_rect.x = 4 * 64;
  enemyFlecheData->src_rect.y = 0;
  enemyFlecheData->src_rect.w = 64;
  enemyFlecheData->src_rect.h = 64;

  enemyFlecheData->dst_rect.x = x;
  enemyFlecheData->dst_rect.y = y;
  enemyFlecheData->dst_rect.w = 64 * 2;
  enemyFlecheData->dst_rect.h = 64 * 2;

  enemyFlecheData->state = FLECHE_MOVING_UP;
  enemyFlecheData->pauseStartBits = 0;
  enemyFlecheData->pauseAttack = 0;
  enemyFlecheData->attackCounter = 0;
  enemyFlecheData->pauseMusic = 0;
}

void flecheAttack(EnemyFlecheData *enemyFlecheData, Perso *perso, Map *map) {
  int intervalAttack = 1000;
  if (hitbox_enemyFleche(perso, map, enemyFlecheData) && enemyFlecheData->state != FLECHE_PAUSE_BOTTOM){
      if (SDL_GetTicks() - enemyFlecheData->pauseAttack >= intervalAttack && perso->health >= 0){
          perso->health -= 1;
          enemyFlecheData->pauseAttack = SDL_GetTicks();
      }
        }
  }

int hitbox_enemyFleche(Perso *perso, Map *map, EnemyFlecheData *enemy) {
    SDL_Rect enemyHitbox = enemy->dst_rect;
    int margin = 30;
    enemyHitbox.y += 2*margin;
    enemyHitbox.h -= 2*margin;
    enemyHitbox.h -= 1*margin;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}

void updatePersoEnemyFleche(Perso *perso, Map *map, EnemyFlecheData *enemy){
    if (!isBossMap){
        if (hitbox_enemyFleche(perso, map, enemy) && enemy->state != FLECHE_PAUSE_BOTTOM){
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
               if (dy > 0 ) { // The character is moving downwards and hasn't jumped yet
                // Make the character bounce above the enemy
                perso->vy = -JUMPSPEED;
            }
        }
    }
}
