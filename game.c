#include "game.h"
#include "enemy1.h"
#include "map.h"
#include "projectile.h"
#include "boss.h"
#include "perso.h"
#include "fight.h"
#include "menu.h"

bool firstTime1 = true;
bool firstTime2 = true;
bool firstTime3 = true;

void game(EnemyStateData enemyStateData, Boss *boss,Map *map, Perso *perso,const Uint8 *state, Mix_Chunk **sounds) {
    if (!afficherImage && !isBossMap) {
        perso->vx = 0;
        updateCam(perso, map);
        updatePerso(perso, map, &enemyStateData, boss,state,sounds);
    }
    if ((afficherImage || parametre) && isBossMap) {
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

        // Me permet de tester l'attaque finale du boss
        static int b_key_down = 0; 

        if (state[SDL_SCANCODE_B]) {
            if (!b_key_down) {
                if (boss->health > 0) {
                    damageBossBallon(boss);
                }
            }
            b_key_down = 1;
        } else {
            b_key_down = 0;
        }
        // 

        updatePerso(perso, map, &enemyStateData,boss,state,sounds);
        if (boss -> health > 0) {
            if (currentTime1 - lastBossMoveTime >= bossMove) {
                updateBoss(boss, map);
                lastBossMoveTime = currentTime1;
            }
        }
        updateCam(perso, map);
        if (boss -> health > 0) {
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
            for (int j = 0 ; j < MAX_PROJECTILES ; j++) {
                for (int k = 0 ; k < MAX_PROJECTILES ; k++) {
                    for (int l = 0 ; l < MAX_PROJECTILES ; l++) {
                        for (int m = 0 ; m < MAX_PROJECTILES ; m++) {
                            if (projectiles[j].active == false && projectiles[k].active == false && j != k && projectiles[l].active == false && l != k && l != j && projectiles[m].active == false && m != k && m != j && m != l) {
                                if (boss -> health == 1 && firstTime1) {
                                    spawnProjectile(j, 13, 5,perso->x*map->pix_rect, perso->x*map->pix_rect, map);
                                    spawnProjectile(k, 19, 4, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    spawnProjectile(l, 11, 13, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    spawnProjectile(m, 18, 12, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    firstTime1 = false;
                                }
                                if (boss -> health == 2 && firstTime2) {
                                    spawnProjectile(j, 5, 3,perso->x*map->pix_rect, perso->x*map->pix_rect, map);
                                    spawnProjectile(k, 5, 7, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    spawnProjectile(l, 23, 2, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    spawnProjectile(m, 22, 7, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    firstTime2 = false;
                                }
                                if (boss -> health == 3 && firstTime3) {
                                    spawnProjectile(j, 26, 14,perso->x*map->pix_rect, perso->x*map->pix_rect, map);
                                    spawnProjectile(k, 8, 11, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    spawnProjectile(l, 7, 15, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    spawnProjectile(m, 15, 9, perso->x * map->pix_rect, perso->x * map->pix_rect, map);
                                    firstTime3 = false;
                                }
                            }
                        }
                    }
                }
            }
            updateProjectile(&projectiles[0],perso,perso->x*map->pix_rect, perso->y*map->pix_rect, map,boss);
        }
    }
}


void game2(SDL_Renderer *renderer, PersoFight *playerInFight, bossFight *boss, AttackFight *nullAttack1, AttackFight *nullAttack2, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, AttackFight *attack5, AttackFight *attack6) {
    fightBoss(renderer, boss, playerInFight, nullAttack1, nullAttack2, attack1, attack2, attack3, attack4, attack5, attack6);
    fightMovement(renderer, playerInFight);
}
