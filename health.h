#ifndef HEALTH_H
#define HEALTH_H
#include "const.h"
#include "boss.h"


void renderStatusHealth(SDL_Renderer *renderer, Perso *perso);
void renderStatusHealthFight(SDL_Renderer *renderer, PersoFight *perso);
void renderStatusBoss(SDL_Renderer *renderer, Boss *boss);
void revive(Perso *perso);


#endif
