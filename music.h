#ifndef MUSIC_H
#define MUSIC_H
#include <SDL2/SDL_mixer.h>
#define MAX_VOLUME MIX_MAX_VOLUME
#define MIN_VOLUME 0
#define MAX_HEARING_DISTANCE 600.0
/* #include "perso.h" */

extern Mix_Chunk *musicEnemyFleche;
extern Mix_Chunk *musicEnemySnake;
extern Mix_Chunk *musicEnemyBat;
extern Mix_Chunk *musicEnemyBatAttack;

int load_music();
int free_music();
/* void adjustVolumeBasedOnDistance(Perso *perso, int channel, double soundSourceX); */



#endif
