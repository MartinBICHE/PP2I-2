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
void cleanupProjectiles(void);
void printProjectiles(Projectile* projectiles);
bool checkProjectileCollisionWithPerso(Projectile* projectile, Perso* perso);
void checkProjectileCollisionWithTiles(Projectile* projectile, Map* map);
void resetProjectiles(void);
void freeProjectileTexture(void);

struct _ProjectileData{
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pause;
};
typedef struct _ProjectileData ProjectileData;
void initProjectile(int x, int y, ProjectileData *projectile);
void projectile_mouvement(SDL_Renderer *renderer, ProjectileData *projectile, Map *map);

#endif /* PROJECTILE_H */
