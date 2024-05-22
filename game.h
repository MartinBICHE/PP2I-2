#ifndef GAME_H
#define GAME_H

#include "const.h"
#include "enemy1.h"
#include "boss.h"

void game(EnemyStateData enemyStateData, Boss *boss,Map *map, Perso *perso,const Uint8 *state);

#endif /* GAME_H */
