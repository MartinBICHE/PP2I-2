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
#include "attack.h"
#include "projectile.h"
#include <SDL2/SDL_mixer.h>

typedef struct _ProjectileData ProjectileData; // Forward declaration

Perso *create_perso(Map*);
void loadPersoTexture(SDL_Renderer*, SDL_Texture**, char*);
void loadAttackTexture(SDL_Renderer *renderer, SDL_Texture **attackTexture, char *path);
int display_perso(SDL_Renderer*, Perso*, Map*, SDL_Texture*, int, Mix_Chunk**);
int hitbox_bottom(Perso*, Map*);
int hitbox_top(Perso*, Map*);
int hitbox_left(Perso*, Map*);
int hitbox_right(Perso*, Map*);
int hitbox_enemy(Perso *perso, Map *map, EnemyStateData *enemyStateData);
float max(float, float);
float min(float, float);
void updatePerso(Perso*, Map*, EnemyStateData*, const Uint8*, Mix_Chunk**, AttackData*, ProjectileData*);
void jump(Perso*, Map*);
void persoAttack(Perso *perso, EnemyStateData *enemyStateData);
void distanceAttack(Perso *perso, EnemyStateData *enemyStateData, Map *map, ProjectileData *projectile);
void changeGravity(void);

#endif
