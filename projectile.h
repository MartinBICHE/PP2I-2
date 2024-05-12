#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "const.h"
#include "perso.h"


bool isValidPosition(Map* map, float x, float y);
void updateProjectile(Projectile* projectiles,Perso* perso, float targetX, float targetY, Map* map);
void renderProjectiles(SDL_Renderer* renderer);
SDL_Texture* loadProjectileTexture(SDL_Renderer* renderer);
void renderProjectile(float x, float y, SDL_Renderer* renderer);
void spawnProjectile(int indice, int startTileX, int startTileY, float targetX, float targetY, Map* map);
void cleanupProjectiles();
void printProjectiles(Projectile* projectiles);
bool checkProjectileCollisionWithPerso(Projectile* projectile, Perso* perso);

#endif /* PROJECTILE_H */
