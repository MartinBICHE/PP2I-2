#ifndef HEALTH_H
#define HEALTH_H
#include "const.h"


void renderStatusHealth(SDL_Renderer *renderer, Perso *perso);
void renderStatusHealthFight(SDL_Renderer *renderer, PersoFight *perso);
void revive(Perso *perso);


#endif
