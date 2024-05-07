#include "enemyFleche.h"
#include "perso.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mixer.h>

/* enemyFleche_mouvement(renderer, &enemyFlecheData, x_cam); */
/* EnemyFlecheData enemyFlecheData; */
/* initEnemyFleche(&enemyFlecheData, x(à définir), y(à définir)); */

void enemyFleche_mouvement(SDL_Renderer *renderer,
                           EnemyFlecheData *enemyFlecheData, float x_cam) {
  int interval = 200;
  int pauseInterval = 3000;

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
    if (SDL_GetTicks() - enemyFlecheData->pauseStart >= pauseInterval) {
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
      enemyFlecheData->dst_rect.x - x_cam, enemyFlecheData->dst_rect.y,
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
  enemyFlecheData->dst_rect.w = 64 * 3;
  enemyFlecheData->dst_rect.h = 64 * 3;


  enemyFlecheData->state = FLECHE_MOVING_UP;
  enemyFlecheData->pauseStartBits = 0;
  enemyFlecheData->pauseAttack = 0;
  enemyFlecheData->attackCounter = 0;
}

void flecheAttack(EnemyFlecheData *enemyFlecheData, Perso *perso) {
  int pad = 49;
  int intervalAttack = 1000;
  if ((perso->x * PIX_RECT - pad <= enemyFlecheData->dst_rect.x + 3 * 64) &&
      (perso->x * PIX_RECT - pad >= enemyFlecheData->dst_rect.x)) {
      if (enemyFlecheData->state != FLECHE_PAUSE_BOTTOM && perso->health > 0){
              if (SDL_GetTicks() - enemyFlecheData->pauseAttack >= intervalAttack){
                  perso->health -= 1;
                  enemyFlecheData->pauseAttack = SDL_GetTicks();
              }
          }
      }
}
