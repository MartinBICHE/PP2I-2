#ifndef MUSIC_H
#define MUSIC_H
#include <SDL2/SDL_mixer.h>

extern Mix_Chunk *musicEnemyFleche;
extern Mix_Chunk *musicEnemySnake;
extern Mix_Chunk *musicEnemyBat;
extern Mix_Chunk *musicEnemyBatAttack;

int load_music();
int free_music();



#endif
