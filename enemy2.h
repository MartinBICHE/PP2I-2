#ifndef enemy2_h
#define enemy2_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "graph.h"
#include "textures.h"


enum Enemy2State{
    EYE_MOVING_RIGHT,
    EYE_MOVING_LEFT
};

struct _Enemy2{
    enum Enemy2State state;
    int health;
    SDL_Rect dst_rect;
    SDL_Rect src_rect;
    Uint32 pauseStartBits;
    Uint32 pauseAttack;
    Node *goal;
    Node *start;
};
typedef struct _Enemy2 Enemy2;


void enemy2_follow(SDL_Renderer *renderer, Enemy2 *enemy, Node **graph, Map *map);
void enemy2Attack(Enemy2 *enemy, Perso *perso, Map *map);
int hitbox_enemy2(Perso *perso, Map *map, Enemy2 *enemy);
void updatePersoEnemy2(Perso *perso, Map *map, Enemy2 *enemy);
void initEnemy2(Enemy2 *enemy, Node *start, Node *goal, Map *map);

#endif
