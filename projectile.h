#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "const.h"
#include "perso.h"
#include "enemy1.h"


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
    int distance_attack_active;
    Uint32 last_distance_attack;
    int dirX; // Direction du projectile (1 pour droite, -1 pour gauche)
};
typedef struct _ProjectileData ProjectileData;
void initProjectile(int x, int y, int dirX, ProjectileData *projectile);
void projectile_mouvement(SDL_Renderer *renderer, ProjectileData *projectile, EnemyStateData *enemyStateData, Map *map);
int hitbox_projectile_enemy(ProjectileData *projectile, EnemyStateData *enemyStateData);

#endif /* PROJECTILE_H */
