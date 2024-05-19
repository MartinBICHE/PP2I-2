#ifndef PERSO_H
#define PERSO_H
#include "map.h"


Perso *create_perso(Map*);
int display_perso(SDL_Renderer*, Perso*, Map*);
int hitbox_bottom(Perso*, Map*);
int hitbox_top(Perso*, Map*);
int hitbox_left(Perso*, Map*);
int hitbox_right(Perso*, Map*);
float max(float, float);
float min(float, float);
void updatePerso(Perso*, Map*);
void jump(Perso*, Map*);
void changeGravity(void);
// void destroy_perso(Perso *);


#endif
