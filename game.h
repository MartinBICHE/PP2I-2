#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_mixer.h>
#include "const.h"
#include "enemy1.h"
#include "enemyFleche.h"
#include "boss.h"

void game(Enemy2 *enemy2, Enemy3 *enemy3, EnemyBatData *enemyBatData, Boss *boss,Map *map, Perso *perso,const Uint8 *state, Mix_Chunk **sounds, AttackData *attackData, ProjectileData *projectile);
void game2(SDL_Renderer *renderer, PersoFight *playerInFight, bossFight *boss, AttackFight *nullAttack1, AttackFight *nullAttack2, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, AttackFight *attack5, AttackFight *attack6);

#endif /* GAME_H */
