#ifndef FIGHT_H
#define FIGHT_H



int showRectangle(SDL_Renderer*, int, int, int, int, Uint32);
int fightMovement(SDL_Renderer*, SDL_Event, Perso*);
int lowRangeAttack(SDL_Renderer*);

#endif
