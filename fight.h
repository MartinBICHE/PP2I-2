#ifndef FIGHT_H
#define FIGHT_H
#include <SDL2/SDL.h>
#include "const.h"

/* Fonction de gestion du personnage */

int fightMovement(SDL_Renderer*, PersoFight*);
void takeDamage(AttackFight*, PersoFight*);
void invincibility(PersoFight*);
void damageBoss(AttackFight*, PersoFight*, bossFight*);
void attackTheBoss(SDL_Renderer* , AttackFight*, bossFight*, PersoFight*, Animation*);

/* Fonction d'initialisation d'attaque */

AttackFight* initAttack(int, int, bossFight*);
void resetAttack(AttackFight*, int);

/* Fonction d'animation */

void loadDeathTexture(SDL_Renderer *renderer, SDL_Texture **attackTexture, char *path);
void initAnimation(Animation* anim, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Uint32 frameDuration);
void updateAnimation(Animation* anim);
void resetAnimation(Animation*);
void renderAnimation(SDL_Renderer* renderer, Animation* anim, int x, int y);
void renderLoopAnimation(SDL_Renderer* renderer, Animation* anim, int x, int y);
void renderBackgroundAnimation(SDL_Renderer* renderer, Animation* anim);
void animateBackground(SDL_Renderer*, Animation*);


/* Fonction d'affichage d'attaque */

int showAttack(SDL_Renderer*, AttackFight*, Animation *);
int showHitPoint(SDL_Renderer*, AttackFight*, Animation *);

/* Fonction de pattern d'attaque */

int threeRowAttacks(SDL_Renderer*, PersoFight*, AttackFight*, AttackFight*, AttackFight*, bossFight*, Animation*);
int threeTimedAttack(SDL_Renderer*, PersoFight*, AttackFight*, AttackFight*, AttackFight*, bossFight*, Animation*);

/* Implémentation des combats de boss */

int fightBoss(SDL_Renderer*, bossFight*, PersoFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, Animation*, Animation*);
void resetGameplay2(bossFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*);


#endif
