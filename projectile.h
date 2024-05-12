#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "const.h"

// void initProjectile(Projectile* projectile, float x, float y, float vx, float vy);
bool isValidPosition(Map* map, float x, float y);
void updateProjectile(Projectile* projectiles, float targetX, float targetY, Map* map);
void renderProjectiles(SDL_Renderer* renderer);
SDL_Texture* loadProjectileTexture(SDL_Renderer* renderer);
void renderProjectile(float x, float y, SDL_Renderer* renderer);
void spawnProjectile(int indice, int startTileX, int startTileY, float targetX, float targetY, Map* map);
void cleanupProjectiles();
void printProjectiles(Projectile* projectiles);

#endif /* PROJECTILE_H */
