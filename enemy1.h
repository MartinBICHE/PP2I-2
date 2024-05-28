#ifndef enemy1_h
#define enemy1_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
/* #include "perso.h" */
#include "map.h"
#define INIT_ENEMY1(enemy, map, x, y) initEnemy1((x) * (map)->pix_rect, (y) * (map)->pix_rect, enemy)



enum EnemyState {
    MOVING_UP,
    PAUSE_TOP,
    MOVING_DOWN,
    PAUSE_BOTTOM,
    ANIMATION_START
};

struct _EnemyStateData{
    enum EnemyState state;
    Uint32 pauseStartBits;
    Uint32 pauseStart;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseAttack;
    Uint32 pauseMusic;
}; 

/* à mettre health dans EnemyState au pire */

typedef struct _EnemyStateData EnemyStateData;

void initEnemy1(int x, int y, EnemyStateData *enemyStateData);
void enemy1_movement(SDL_Renderer *renderer, EnemyStateData *enemyStateData, Map *map);
void enemy1Attack(EnemyStateData *enemyStateData, Perso *perso, Map *map);
void updatePersoEnemy1(Perso *perso, Map *map, EnemyStateData *enemyStateData);
int hitbox_enemy1(Perso *perso, Map *map, EnemyStateData *enemyStateData);
#endif
