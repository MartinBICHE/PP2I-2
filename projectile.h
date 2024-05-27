#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "const.h"
#include "perso.h"


bool isValidPosition(Map* map, float x, float y);
void updateProjectile(Projectile* projectiles,Perso* perso, float targetX, float targetY, Map* map,Boss* boss);
void renderProjectiles(SDL_Renderer* renderer,Map *map);
void loadProjectileTexture(SDL_Renderer* renderer, SDL_Texture **projectileTexture, char *path);
int renderProjectile(SDL_Renderer* renderer, Projectile *projectile, Map *map, SDL_Texture *bossTexture, int showHitbox);
void spawnProjectile(int indice, int startTileX, int startTileY, float targetX, float targetY, Map* map);
void cleanupProjectiles(void);
void printProjectiles(Projectile* projectiles);
bool checkProjectileCollisionWithPerso(Projectile* projectile, Perso* perso);
bool checkProjectileCollisionWithBoss(Projectile* projectile, Boss* boss);
void checkProjectileCollisionWithTiles(Projectile* projectile, Map* map);
void resetProjectiles(void);
void freeProjectileTexture(void);
int display_projectile_hitbox(SDL_Renderer *renderer, Projectile *projectile, Map *map);

#endif /* PROJECTILE_H */
