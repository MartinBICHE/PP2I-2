#ifndef enemybat_h
#define enemybat_h
#include "enemyFleche.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

enum EnemyBatState{
    BAT_MOVING_RIGHT,
    BAT_MOVING_LEFT,
    BAT_ATTACK
};

struct _EnemyBatData{
    enum EnemyBatState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    SDL_Rect src_rectAttack;
    SDL_Rect dst_rectAttack;
    Uint32 pauseStartBits;
    Uint32 pauseStartBitsAttack;
    int xMax;
    int xMin;
    Uint32 pauseAttack;
    Uint32 pauseMusic;
    enum EnemyBatState previousState;
};
typedef struct _EnemyBatData EnemyBatData;
void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax);
void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Map *map);
void batAttack(EnemyBatData *enemyBatData, Perso *perso, Map *map);

#endif
