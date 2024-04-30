#ifndef enemy1_h
#define enemy1_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

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
}; 

/* à mettre health dans EnemyState au pire */

typedef struct _EnemyStateData EnemyStateData;

void enemy1_movement(SDL_Renderer *renderer, SDL_Texture *texture, EnemyStateData *enemyStateData);
void initEnemy1(int x, int y, EnemyStateData *enemyStateData);
#endif
