#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemyBat.h"
#include "enemyFleche.h"
#include "fight.h"
#include "fonts.h"
#include "health.h"
#include "init.h"
#include "map.h"
#include "pendule.h"
#include "perso.h"
#include "scroll.h"
#include "textures.h"
#include "projectile.h"
#include "boss.h"
#include "menu.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textures.h"
#include "fonts.h"
#include "health.h"
#include "enemyFleche.h"
#include "enemyBat.h"
#include "fight.h"
#include "attack.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;
SDL_Texture *bgTextures[6];
Mix_Chunk *sounds[4];
SDL_Texture *persoTexture;
SDL_Texture *tileTextures;
SDL_Texture* projectileTexture = NULL;
bool showMenu = true;
bool parametre = false;
bool afficherImage = false;
bool retourMenu = false;
bool startGame = false;
bool prevShowMenu = true;
bool musicToggled = false;
bool quit = false;
bool running = true;
float currentGravity = ACC;
float jumpSpeed = JUMPSPEED;
bool showAttentionImage = true;
bool firstIteration = true;

Uint32 lastGravityChange = 0;
Uint32 lastProjectileLoad = 0;
Uint32 lastBossMoveTime = 0;
Uint32 boutonGTime = 0;
Uint32 currentTime1 = 0;

Uint32 pauseStartTime = 0;
Uint32 totalPauseDuration = 0;

bool isBossMap = false;

// Initialisation du tableau de projectiles
Projectile projectiles[MAX_PROJECTILES] = {
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 1
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 2
    {0.0f, 0.0f, 0.0f, 0.0f, false}  // Projectile 3
};



int main(int argc, char **argv) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);
    // const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

    PersoFight *playerInFight = (PersoFight*)malloc(sizeof(PersoFight));
	playerInFight->y = QUARTERHEIGHT-1.5*SPRITESIZE/2;
	playerInFight->x = TIERWIDTH/2-1.5*SPRITESIZE/2;
	playerInFight->health = 9;
	playerInFight->iframe = 0;

	/* Création du boss */

	bossFight *bossDeath = (bossFight*)malloc(sizeof(bossFight));
	bossDeath->health = 9;
	bossDeath->phase = 1;
	bossDeath->delay = 200;
	bossDeath->attack1Delay = 3*bossDeath->delay;
	bossDeath->attack2Delay = 3*bossDeath->delay;
	bossDeath->attack3Delay = 3*bossDeath->delay;
	bossDeath->speed = 2;

	/* Déclaration des attaques pour le gameplay 2 */

	AttackFight *nullAttack1 = initAttack(3*TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *nullAttack2 = initAttack(3*TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack1 = initAttack(0, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack2 = initAttack(TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack3 = initAttack(2*TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack4 = initAttack(0, 0, bossDeath);
	AttackFight *attack5 = initAttack(TIERWIDTH, 0, bossDeath);
	AttackFight *attack6 = initAttack(2*TIERWIDTH, 0, bossDeath);
    
    Map *map = initMap("map2");
	Perso *perso = create_perso(map);
    Boss *boss = NULL;

    CheckpointList *checkpointList = malloc(sizeof(CheckpointList));
    initCheckpointList(checkpointList);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int running = 1;


    loadBackgroundTextures(renderer, bgTextures, 5);
    loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
    // loadPersoTexture(renderer, &persoTexture, "./asset/spritesheet/ss_mc.png");



     /////////////////////////////////////////////////////* Les init des ennemis *////////////////////////////////////////////////////////////////////////
     
    /////////* Le graph pour A* *///////////:
    Node **graph = create_graph(map);
    
    /////////////////////////////////////////////////////////* Déclaration enemis *///////////////////////////////////////////////////////////////////

    EnemyStateData enemyStateData;
    EnemyStateData enemyStateData1;
    EnemyStateData enemyStateData2;
    EnemyStateData enemyStateData3;
    EnemyPenduleData enemyPenduleData;
    EnemyPenduleData enemyPenduleData1;
    EnemyPenduleData enemyPenduleData2;
    EnemyBatData enemyBatData;
    EnemyBatData enemyBatData1;
    EnemyBatData enemyBatData2;
    EnemyBatData enemyBatData3;
    EnemyBatData enemyBatData4;
    EnemyBatData enemyBatData5;
    EnemyBatData enemyBatData6;
    EnemyFlecheData enemyFlecheData;
    EnemyFlecheData enemyFlecheData1;
    EnemyFlecheData enemyFlecheData2;
    EnemyFlecheData enemyFlecheData3;
    EnemyFlecheData enemyFlecheData4;
    EnemyFlecheData enemyFlecheData5;
    EnemyFlecheData enemyFlecheData6;
    EnemyFlecheData enemyFlecheData7;
    EnemyFlecheData enemyFlecheData8;
    EnemyFlecheData enemyFlecheData9;
    EnemyFlecheData enemyFlecheData10;
    Enemy3 enemy3;
    Enemy3 enemy31;
    Enemy3 enemy32;
    Enemy3 enemy33;
    Enemy3 enemy34;
    Enemy2 enemy2;
    Enemy2 enemy21;

    //////////////////////////////////////////////////:/* Les fonctions d'init pour chaque *////////////////////////////////////////////////////////:

    INIT_ENEMY1(&enemyStateData, map, 26, 14);
    INIT_ENEMY1(&enemyStateData1, map, 103, 15);
    INIT_ENEMY1(&enemyStateData2, map, 118, 15);
    INIT_ENEMY1(&enemyStateData3, map, 183, 15);
    INIT_ENEMYPENDULE(&enemyPenduleData, map, 46, 3);
    INIT_ENEMYPENDULE(&enemyPenduleData1, map, 129, 1);
    INIT_ENEMYPENDULE(&enemyPenduleData2, map, 169, 1);
    INIT_ENEMYBAT(&enemyBatData, map, 15, 7, 21);
    INIT_ENEMYBAT(&enemyBatData1, map, 57, 4, 63);
    INIT_ENEMYBAT(&enemyBatData2, map, 35, 7, 39);
    INIT_ENEMYBAT(&enemyBatData2, map, 108, 5, 113);
    INIT_ENEMYBAT(&enemyBatData3, map, 122, 9, 126);
    INIT_ENEMYBAT(&enemyBatData4, map, 132, 7, 137);
    INIT_ENEMYBAT(&enemyBatData5, map, 195, 12, 234);
    INIT_ENEMYBAT(&enemyBatData6, map, 246, 9, 253);
    INIT_ENEMYFLECHE(&enemyFlecheData, map, 15, 11);
    INIT_ENEMYFLECHE(&enemyFlecheData1, map, 40, 12);
    INIT_ENEMYFLECHE(&enemyFlecheData2, map, 47, 12);
    INIT_ENEMYFLECHE(&enemyFlecheData3, map, 75, 6);
    INIT_ENEMYFLECHE(&enemyFlecheData4, map, 97, 6);
    INIT_ENEMYFLECHE(&enemyFlecheData5, map, 110, 13);
    INIT_ENEMYFLECHE(&enemyFlecheData6, map, 113, 13);
    INIT_ENEMYFLECHE(&enemyFlecheData7, map, 166, 13);
    INIT_ENEMYFLECHE(&enemyFlecheData8, map, 169, 13);
    INIT_ENEMYFLECHE(&enemyFlecheData9, map, 163, 13);
    INIT_ENEMYFLECHE(&enemyFlecheData10, map, 255, 5);
    INIT_ENEMY3(&enemy3, map, 27, 12, 39);
    INIT_ENEMY3(&enemy31, map, 67, 12, 78);
    INIT_ENEMY3(&enemy32, map, 141, 14, 146);
    INIT_ENEMY3(&enemy33, map, 172, 13, 180);
    INIT_ENEMY3(&enemy34, map, 185, 13, 195);
    Node *startEnemy2 = &graph[12][77];
    Node *goalEnemy2 = &graph[14][95];
    Node *startEnemy21 = &graph[8][231];
    Node *goalEnemy21 = &graph[14][246];
    initEnemy2(&enemy2, startEnemy2, goalEnemy2, map);
    initEnemy2(&enemy21, startEnemy21, goalEnemy21, map);








    ///////////////////////////////////////////////////* fin init des ennemis *////////////////////////////////////////////////////////////////////////:


    // Initialiser SDL_mixer
    /* if (!initSDL_mixer()) { */
    /*     SDL_Log("Erreur lors de l'initialisation de SDL_mixer."); */
    /*     return 1; */
    /* } */

    /* // Charger la musique */
    /* if (!loadMusic()) { */
    /*     SDL_Log("Erreur lors du chargement de la musique."); */
    /*     closeSDL_mixer(); */
    /*     return 1; */
    /* } */

    loadSounds(sounds);

    // Jouer la musique lorsque le menu s'ouvre
    playMusic();

again :

    retourMenu = false;
    startGame = false;
    prevShowMenu = true;
    if (perso -> health == 0) {
        revive(perso); // revive ne marche pas créer une fonction resetGame qui renvoie au point de départ
    }
    // Boucle principale du menu
    quit = false;
    while (!quit) {
        interactionMenu(renderer);   
        if (startGame) {
            if (!map) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors du chargement de la carte.");
                return -1;
            }

            loadBackgroundTextures(renderer, bgTextures, 5);
            loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
            loadPersoTexture(renderer, &persoTexture, "./asset/spritesheet/ss_mc.png");

            while (running) {
                Uint64 start = SDL_GetTicks();
                while (SDL_PollEvent(&e) != 0) {
                    interactionPauseJeu(renderer);
                
                    if (e.key.keysym.sym == SDLK_SPACE && !afficherImage && perso->recoil_timer <= 0) {
                        jump(perso, map);
                    } else if (e.key.keysym.sym == SDLK_g && !afficherImage) {
                        boutonGTime = SDL_GetTicks();
                        isBossMap = true;
                        destroyMap(map);
                        free(perso);
                        map = initMap("mapBoss1");
                        perso = create_perso(map);
                        boss = create_boss(map);
                        lastGravityChange = currentTime1;
                        lastProjectileLoad = currentTime1;
                        lastBossMoveTime = currentTime1;
                    } else if (e.key.keysym.sym == SDLK_p && !afficherImage) {
                        perso -> health = 0;
                    }
                    
                }   
                if (retourMenu) {
                    goto again;
                } 
                    

                

                // x_cam = updateCamm(perso->x*PIX_RECT, x_cam);
                if (perso-> health > 0) {
                    game(enemyStateData, boss, map, perso, state, sounds);
                    if (drawBackground(renderer, bgTextures, 5, map)) {
                        printf("Error drawing the background");
                        exit(-1);
                    }
                    if (drawMap(renderer, map, tileTextures)) {
                        printf("Error drawing the map");
                        exit(-1);
                    }
                    if (display_perso(renderer, perso, map, persoTexture, 0, sounds)) {
                        printf("Error drawing the perso");
                        exit(-1);
                    }
                    if (!isBossMap) {
                        /////////////////////////////////* les mouvements de chaque ennemi *////////////////////////////////////////////////
                        enemy1_movement(renderer, &enemyStateData, map);
                        enemy1Attack(&enemyStateData, perso, map);
                        updatePersoEnemy1(perso, map, &enemyStateData);

                        enemy1_movement(renderer, &enemyStateData1, map);
                        enemy1Attack(&enemyStateData1, perso, map);
                        updatePersoEnemy1(perso, map, &enemyStateData1);

                        enemy1_movement(renderer, &enemyStateData2, map);
                        enemy1Attack(&enemyStateData2, perso, map);
                        updatePersoEnemy1(perso, map, &enemyStateData2);

                        enemy1_movement(renderer, &enemyStateData3, map);
                        enemy1Attack(&enemyStateData3, perso, map);
                        updatePersoEnemy1(perso, map, &enemyStateData3);


                        enemy2_follow(renderer, &enemy2, graph, map);
                        updatePersoEnemy2(perso, map, &enemy2);
                        enemy2Attack(&enemy2, perso, map);

                        enemy2_follow(renderer, &enemy21, graph, map);
                        updatePersoEnemy2(perso, map, &enemy21);
                        enemy2Attack(&enemy21, perso, map);

                        SDL_Rect enemyHitbox = enemyPenduleData.dst_rect;
                        int margin = 40;
                        enemyHitbox.w -= 3*margin;
                        enemyHitbox.h -= 2*margin;
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
                        SDL_RenderDrawRect(renderer, &enemyHitbox);

                        enemy3_movement(renderer, &enemy3, map);
                        updatePersoEnemy3(perso, map, &enemy3);
                        enemy3Attack(&enemy3, perso, map);

                        enemy3_movement(renderer, &enemy31, map);
                        updatePersoEnemy3(perso, map, &enemy31);
                        enemy3Attack(&enemy31, perso, map);

                        enemy3_movement(renderer, &enemy32, map);
                        updatePersoEnemy3(perso, map, &enemy32);
                        enemy3Attack(&enemy32, perso, map);

                        enemy3_movement(renderer, &enemy33, map);
                        updatePersoEnemy3(perso, map, &enemy33);
                        enemy3Attack(&enemy33, perso, map);

                        enemy3_movement(renderer, &enemy34, map);
                        updatePersoEnemy3(perso, map, &enemy34);
                        enemy3Attack(&enemy34, perso, map);



                        enemyBat_mouvement(renderer, &enemyBatData, map, perso);
                        batAttack(&enemyBatData, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData);

                        enemyBat_mouvement(renderer, &enemyBatData1, map, perso);
                        batAttack(&enemyBatData1, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData1);

                        enemyBat_mouvement(renderer, &enemyBatData2, map, perso);
                        batAttack(&enemyBatData2, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData2);

                        enemyBat_mouvement(renderer, &enemyBatData3, map, perso);
                        batAttack(&enemyBatData3, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData3);

                        enemyBat_mouvement(renderer, &enemyBatData4, map, perso);
                        batAttack(&enemyBatData4, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData4);

                        enemyBat_mouvement(renderer, &enemyBatData5, map, perso);
                        batAttack(&enemyBatData5, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData5);

                        enemyBat_mouvement(renderer, &enemyBatData6, map, perso);
                        batAttack(&enemyBatData6, perso, map);
                        updatePersoEnemyBat(perso, map, &enemyBatData6);



                        enemyFleche_mouvement(renderer, &enemyFlecheData, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData);
                        flecheAttack(&enemyFlecheData, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData1, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData1);
                        flecheAttack(&enemyFlecheData1, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData2, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData2);
                        flecheAttack(&enemyFlecheData2, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData3, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData3);
                        flecheAttack(&enemyFlecheData3, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData4, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData4);
                        flecheAttack(&enemyFlecheData4, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData5, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData5);
                        flecheAttack(&enemyFlecheData5, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData6, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData6);
                        flecheAttack(&enemyFlecheData6, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData7, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData7);
                        flecheAttack(&enemyFlecheData7, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData8, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData8);
                        flecheAttack(&enemyFlecheData8, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData9, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData9);
                        flecheAttack(&enemyFlecheData9, perso, map);

                        enemyFleche_mouvement(renderer, &enemyFlecheData10, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData10);
                        flecheAttack(&enemyFlecheData10, perso, map);



                        enemyPendule_mouvement(renderer, &enemyPenduleData, map);
                        penduleAttack(&enemyPenduleData, perso, map);

                        enemyPendule_mouvement(renderer, &enemyPenduleData1, map);
                        penduleAttack(&enemyPenduleData1, perso, map);

                        enemyPendule_mouvement(renderer, &enemyPenduleData2, map);
                        penduleAttack(&enemyPenduleData2, perso, map);
                        /* SDL_Rect dst_rect = {300, 300, 64, 64}; */
                        /* SDL_Rect src_rect = {0, 0, 64, 64}; */
                        /* SDL_RenderCopy(renderer, textureBat, &src_rect, &dst_rect); */
                        /* projectile_mouvement(renderer, &projectile, map); */
                        /* attack_mouvement(renderer, &attack, map); */
                        //////////////////////////////* fin mouvements de chaque ennemi *////////////////////////////////////////////////

                    }
                    if (isBossMap) {
                        displayBoss(renderer, boss, map);
                        if (showAttentionImage) {
                            renderImage(renderer,"./asset/UI/attention.png",(WINWIDTH / 2 - ImageAttentionWidth / 2),(WINHEIGHT / 2 - ImageAttentionHeight / 2), ImageAttentionWidth, ImageAttentionHeight);
                        }
                        renderProjectiles(renderer);
                    }
                    renderStatusHealth(renderer,perso);

                    if (perso->health == 0) {
                        gameOver1(renderer, bgTextures, 5, map);
                    }
                } else {
                    
                    game2(renderer, playerInFight, bossDeath, nullAttack1, nullAttack2, attack1, attack2, attack3, attack4, attack5, attack6);
                    renderStatusHealthFight(renderer,playerInFight);
                    invincibility(playerInFight);
                    if (bossDeath->health <= 0) {
                        revive(perso);
                        bossDeath->health = 9;
                        playerInFight->health = 9;
                        bossDeath->phase = 1;
                        resetGameplay2(bossDeath, nullAttack1, nullAttack2, attack1, attack2, attack3, attack4, attack5, attack6);
                    }
                    if (playerInFight -> health == 0) {
                        bossDeath->health = 9;
                        playerInFight->health = 9;
                        bossDeath->phase = 1;
                        resetGameplay2(bossDeath, nullAttack1, nullAttack2, attack1, attack2, attack3, attack4, attack5, attack6);
                        goto again;
                    }
                }
                drawMapMenu(renderer);
                    
                // SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a); // !!! seulement pour les tests de caméra (à changer)
                // SDL_Rect rect1 = {.x = x_perso*PIX_RECT - 10 - x_cam, .y = 3*PIX_RECT - 10, .w = 20, .h = 20}; // !!! seulement pour les tests de caméra (à changer)
                // SDL_RenderDrawRect(renderer, &rect1); // !!! seulement pour les tests de caméra (à changer)
                // SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a); // !!! seulement pour les tests de caméra (à changer)
                // SDL_Rect rect2 = {.x = x_perso*PIX_RECT - 9 - x_cam, .y = 3*PIX_RECT - 9, .w = 18, .h = 18}; // !!! seulement pour les tests de caméra (à changer)
                // SDL_RenderDrawRect(renderer, &rect2); // !!! seulement pour les tests de caméra (à changer)

                

                SDL_RenderPresent(renderer);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                Uint64 end = SDL_GetTicks();
                float elapsedMS = (end - start);
                SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
            
            }
        }
    }
    quitSDL(&renderer, &window, perso, map, boss);
    free(checkpointList->xPositions);
    free(checkpointList);
    free(boss);
	free(nullAttack1);
    free(nullAttack2);
	free(attack1);
	free(attack2);
	free(attack3);
	free(attack4);
	free(attack5);
	free(attack6);
    free(playerInFight);
	free(bossDeath);
    cleanupProjectiles();
    /* closeSDL_mixer(); */
    atexit(SDL_Quit);
    return 0;
    
}
