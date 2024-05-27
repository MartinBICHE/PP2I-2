#ifndef ATTACK_H
#define ATTACK_H

#include <SDL2/SDL.h>
#include "const.h"
#include "textures.h"

struct _AttackData{
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pause;
    int attack_active;
    Uint32 last_attack;
};

typedef struct _AttackData AttackData;

void attack_mouvement(SDL_Renderer *renderer, AttackData *attack, Map *map);
void initAttackAnimation(int x, int y, AttackData *attack);

#endif
