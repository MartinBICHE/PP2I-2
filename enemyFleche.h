#ifndef enemyFleche_h
#define enemyFleche_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "perso.h"


enum EnemyFlecheState{
    FLECHE_MOVING_UP,
    FLECHE_MOVING_DOWN,
    FLECHE_PAUSE_TOP,
    FLECHE_PAUSE_BOTTOM

};

struct _EnemyFlecheData{
    enum EnemyFlecheState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseStartBits;
    Uint32 pauseStart;
    Uint32 pauseAttack;
    int attackCounter;
    Uint32 pauseMusic;

};
typedef struct _EnemyFlecheData EnemyFlecheData;
void initEnemyFleche(EnemyFlecheData *enemyFlecheData, int x, int y);
void flecheAttack(EnemyFlecheData *enemyFlecheData, Perso *perso, Map *map);
void enemyFleche_mouvement(SDL_Renderer *renderer, EnemyFlecheData *enemyFlecheData, Map *map);

#endif
