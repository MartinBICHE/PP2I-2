#ifndef PERSO_H
#define PERSO_H
#include "map.h"
#include "enemy1.h" // Nouveau


Perso *create_perso(Map*);
int display_perso(SDL_Renderer*, Perso*, float);
int hitbox_bottom(Perso*, Map*);
int hitbox_top(Perso*, Map*);
int hitbox_left(Perso*, Map*);
int hitbox_right(Perso*, Map*);
float max(float, float);
float min(float, float);
void updatePerso(Perso*, Map*, EnemyStateData*); // Nouveau paramètre de type EnemyStateData*
void jump(Perso*, Map*);
void destroy_perso(Perso *);


#endif