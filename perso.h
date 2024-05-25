#ifndef PERSO_H
#define PERSO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "const.h"
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
int hitbox_boss(Perso *perso, Map *map, Boss *boss);
float max(float, float);
float min(float, float);
void updatePersoDashing(Perso *perso, Map *map);
void updateHitbox(Perso*, Map*);
void updatePerso(Perso*, Map*, EnemyStateData*,Boss*, const Uint8*, Mix_Chunk**);
void jump(Perso*, Map*);
void changeGravity(void);

#endif
