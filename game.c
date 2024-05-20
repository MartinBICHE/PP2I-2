#include "game.h"
#include "enemy1.h"
#include "map.h"
#include "projectile.h"
#include "boss.h"
#include "perso.h"

void game(EnemyStateData enemyStateData, Boss *boss,Map *map, Perso *perso,Uint8 *state) {
    if (!afficherImage && !isBossMap) {
        perso->vx = 0;
        updateCam(perso, map);
        updatePerso(perso, map, &enemyStateData, state);
    }

    if (afficherImage && isBossMap) {
        if (pauseStartTime == 0) {
            pauseStartTime = SDL_GetTicks();
        }
    } else if (!afficherImage && isBossMap){
        currentTime1 = SDL_GetTicks();
        if (firstIteration) {
            lastGravityChange = boutonGTime;
            lastProjectileLoad = boutonGTime;
            lastBossMoveTime = boutonGTime;
            firstIteration = false;
        }
        if (pauseStartTime != 0) {
            Uint32 pauseEndTime = SDL_GetTicks();
            totalPauseDuration += (pauseEndTime - pauseStartTime);
            pauseStartTime = 0;
            lastGravityChange += totalPauseDuration; 
            lastProjectileLoad += totalPauseDuration; 
            lastBossMoveTime += totalPauseDuration;
            totalPauseDuration = 0;
        }

        updatePerso(perso, map, &enemyStateData, state);
        if (currentTime1 - lastBossMoveTime >= BOSS_MOVE_INTERVAL) {
            updateBoss(boss, map);
            lastBossMoveTime = currentTime1;
        }
        updateCam(perso, map);

        if (currentTime1 - lastGravityChange >= GRAVITY_CHANGE_INTERVAL) {
            changeGravity();
            lastGravityChange = currentTime1; // Mettre à jour le temps du dernier changement de gravité
            showAttentionImage = false;
        }
        if (currentTime1 - lastGravityChange >= GRAVITY_CHANGE_INTERVAL-2500 && currentTime1 - lastGravityChange <= GRAVITY_CHANGE_INTERVAL-2000) {
            showAttentionImage = true;
        } else if (currentTime1 - lastGravityChange >= GRAVITY_CHANGE_INTERVAL-1500 && currentTime1 - lastGravityChange <= GRAVITY_CHANGE_INTERVAL-1000) {
            showAttentionImage = true;
        } else if (currentTime1 - lastGravityChange >= GRAVITY_CHANGE_INTERVAL-500 && currentTime1 - lastGravityChange <= GRAVITY_CHANGE_INTERVAL) {
            showAttentionImage = true;
        } else {
            showAttentionImage = false;
        }

        if (currentTime1 - lastProjectileLoad >= PROJECTILE_LOAD_INTERVAL) {
            for (int i = 0 ; i < MAX_PROJECTILES ; i++) {
                if (projectiles[i].active == false) {
                    spawnProjectile(i, boss->x, boss->y, perso->x*map->pix_rect, perso->y*map->pix_rect, map);
                    lastProjectileLoad = currentTime1;
                    break;
                }
            } 
        }
        updateProjectile(&projectiles[0],perso,perso->x*map->pix_rect, perso->y*map->pix_rect, map);
    }
}