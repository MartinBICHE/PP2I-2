#ifndef FIGHT_H
#define FIGHT_H

/* Fonction de gestion du personnage */

int fightMovement(SDL_Renderer*, SDL_Event, PersoFight*);
void takeDamage(AttackFight*, PersoFight*);
void invincibility(PersoFight*);
void damageBoss(AttackFight*, PersoFight*, bossFight*);
void attackTheBoss(SDL_Renderer* , AttackFight*, bossFight*, PersoFight*);

/* Fonction d'initialisation d'attaque */

AttackFight* initAttack(int, int, bossFight*);
void resetAttack(AttackFight*, int);

/* Fonction d'affichage d'attaque */

int showAttack(SDL_Renderer*, AttackFight*);
int showHitPoint(SDL_Renderer*, AttackFight*);

/* Fonction de pattern d'attaque */

int threeRowAttacks(SDL_Renderer*, PersoFight*, AttackFight*, AttackFight*, AttackFight*,int);
int threeTimedAttack(SDL_Renderer*, PersoFight*, AttackFight*, AttackFight*, AttackFight*, int);

/* Implémentation des combats de boss */

int fightBoss(SDL_Renderer*, bossFight*, PersoFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*, AttackFight*);



#endif
