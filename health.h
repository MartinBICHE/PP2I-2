#ifndef HEALTH_H
#define HEALTH_H
#include "const.h"


void renderStatusHealth(SDL_Renderer *renderer, Perso *perso);
void handleDeath(Perso *perso, const char *filename);



#endif
