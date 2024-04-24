#ifndef PERSO_H
#define PERSO_H

#include "enemy2.h"


Perso *create_perso(Map*);
int display_perso(SDL_Renderer*, Perso*, float);
int hitbox_bottom(Perso*, Map*);
int hitbox_top(Perso*, Map*);
int hitbox_left(Perso*, Map*);
int hitbox_right(Perso*, Map*);
float max(float, float);
float min(float, float);
void updatePerso(Perso*, Map*, Enemy2 *enemies, int numEnemies);
void resetPersoPosition(Perso *perso, Map *map);
void jump(Perso*, Map*);
void destroy_perso(Perso *);


#endif
