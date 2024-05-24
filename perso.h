#ifndef PERSO_H
#define PERSO_H
#include "map.h"
#include "enemy1.h"
#include <SDL2/SDL_mixer.h>



Perso *create_perso(Map*);
void loadPersoTexture(SDL_Renderer*, SDL_Texture**, char*);
int display_perso(SDL_Renderer*, Perso*, Map*, SDL_Texture*, int, Mix_Chunk**);
int hitbox_bottom(Perso*, Map*);
int hitbox_top(Perso*, Map*);
int hitbox_left(Perso*, Map*);
int hitbox_right(Perso*, Map*);
int hitbox_enemy(Perso *perso, Map *map, EnemyStateData *enemyStateData);
float max(float, float);
float min(float, float);
void updatePerso(Perso*, Map*, EnemyStateData*, const Uint8*, Mix_Chunk**);
void jump(Perso*, Map*);
void changeGravity(void);

#endif
