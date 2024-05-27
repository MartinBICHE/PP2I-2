#include "attack.h"


void attack_mouvement(SDL_Renderer *renderer, AttackData *attack, Map *map){
    if (!attack->attack_active) return;

    int interval = 100;

    SDL_Rect dst_rect = {attack->dst_rect.x - map->x_cam, attack->dst_rect.y, attack->dst_rect.w, attack->dst_rect.h};
    if (SDL_GetTicks() - attack->pause >= interval){
        attack->src_rect.x += 16;
        attack->pause = SDL_GetTicks();
    }
    if (attack->src_rect.x == 80){
        //attack->src_rect.x = 0;
        attack->dst_rect.w = 0;
        attack->dst_rect.h = 0;
    }
    SDL_RenderCopy(renderer, textureAttack, &attack->src_rect, &dst_rect);
}

void initAttackAnimation(int x, int y, AttackData *attack){
    attack->src_rect.x = 0;
    attack->src_rect.y = 0;
    attack->src_rect.w = 16;
    attack->src_rect.h = 32;

    attack->dst_rect.x = x;
    attack->dst_rect.y = y;
    attack->dst_rect.w = 16*3;
    attack->dst_rect.h = 32*3;

    attack->pause = 0;
    attack->attack_active = 0;
}
