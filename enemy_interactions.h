#ifndef enemy_interactions_h
#define enemy_interactions_h

#include <stdbool.h>

int isCharacterNearEnemy(Perso *perso, Enemy2 *enemy);
void handleCharacterAttack(Perso *perso, Enemy2 *enemy);
bool checkCollision(Perso *perso, Enemy2 *enemy);

#endif
