#ifndef enemyFleche_h
#define enemyFleche_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>


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

};
typedef struct _EnemyFlecheData EnemyFlecheData;
void enemyFleche_mouvement(SDL_Renderer *renderer, EnemyFlecheData *enemyFlecheData);
void initEnemyFleche(EnemyFlecheData *enemyFlecheData, int x, int y);

#endif
