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


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;
SDL_Texture *bgTextures[6];
Mix_Chunk *sounds[4];
SDL_Texture *persoTexture;
SDL_Texture *bossTexture;
SDL_Texture *scratchTexture;
SDL_Texture *hitpointTexture;
SDL_Texture *bgDeathTexture;
SDL_Texture *tileTextures;
SDL_Texture *projectileTexture;
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
float bossMove = BOSS_MOVE_INTERVAL;
float projectileSpeed = PROJECTILE_SPEED;
bool showAttentionImage = true;
bool firstIteration = true;
bool firstSwicthMusic = true;
bool gameplay2 = false;

Uint32 lastGravityChange = 0;
Uint32 lastProjectileLoad = 0;
Uint32 lastBossMoveTime = 0;
Uint32 boutonGTime = 0;
Uint32 currentTime1 = 0;
Uint32 bossAngry1 = 0;
Uint32 bossAngry2 = 0;
Uint32 bossAngry3 = 0;
Uint32 pauseStartTime = 0;
Uint32 totalPauseDuration = 0;

bool isBossMap = false;

// Initialisation du tableau de projectiles
Projectile projectiles[MAX_PROJECTILES] = {
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 1
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 2
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 3
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 4
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 5
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 6
    {0.0f, 0.0f, 0.0f, 0.0f, false}, // Projectile 7
    {0.0f, 0.0f, 0.0f, 0.0f, false} // Projectile 8
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

    loadDeathTexture(renderer, &scratchTexture, "./asset/spritesheet/scratch.png");
    loadDeathTexture(renderer, &hitpointTexture, "./asset/spritesheet/hitpoint.png");
    loadDeathTexture(renderer, &bgDeathTexture, "./asset/background/satan.png");


	AttackFight *attack1 = initAttack(0, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack2 = initAttack(TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack3 = initAttack(2*TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack4 = initAttack(0, 0, bossDeath);
	AttackFight *attack5 = initAttack(TIERWIDTH, 0, bossDeath);
	AttackFight *attack6 = initAttack(2*TIERWIDTH, 0, bossDeath);
    
    Map *map2 = initMap("map2");
    Map *mapBoss = initMap("mapBoss1");
    Map *map = map2;

    Animation *scratchAnim = malloc(sizeof(Animation));
    Animation *hitpointAnim = malloc(sizeof(Animation));
    Animation *bgDeathAnim = malloc(sizeof(Animation));


	Perso *perso = create_perso(map);
    Boss *boss = create_boss(mapBoss);

    CheckpointList *checkpointList = malloc(sizeof(CheckpointList));
    initCheckpointList(checkpointList);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int running = 1;

    loadMusicGameplay1();
    loadMusicGameplay2();

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

    const char *instructionText = "Instructions:   Appuyez sur Q pour se deplacer a gauche et D pour se deplacer a droite. J pour un dash et Espace pour un saut";

    DialogBoxData instructionBox;
    initPapirus(&instructionBox, 200, 100);
    SDL_Color BLACK = {0, 0, 0, 255};

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

    loadSounds(sounds);

again :
    Mix_HaltMusic();
    retourMenu = false;
    startGame = false;
    prevShowMenu = true;
    if (perso -> health == 0) {
        resetGame(&window, &renderer, &map2, &mapBoss, &perso, &boss);
        currentGravity = ACC;
        map = map2;
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
            loadSounds(sounds);
            loadBackgroundTextures(renderer, bgTextures, 5);
            loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
            loadPersoTexture(renderer, &persoTexture, "./asset/spritesheet/ss_mc.png");
            loadBossTexture(renderer,&bossTexture,"./asset/spritesheet/boss.png");
            loadProjectileTexture(renderer,&projectileTexture,"./asset/spritesheet/tornade.png");
            initAnimation(scratchAnim, scratchTexture, 2400, 160, 10, 150);
            initAnimation(hitpointAnim, hitpointTexture, 960 ,160, 4, 200);
            initAnimation(bgDeathAnim, bgDeathTexture, 12960,480, 18, 100);

            while (running) {
                Uint64 start = SDL_GetTicks();
                while (SDL_PollEvent(&e) != 0) {
                    interactionPauseJeu(renderer, &map2, &mapBoss, &perso, &boss);
                
                    if (e.key.keysym.sym == SDLK_SPACE && !afficherImage && perso->recoil_timer <= 0) {
                        jump(perso, map);
                    } else if (perso->x > 296) {
                        boutonGTime = SDL_GetTicks();
                        isBossMap = true;
                        int health = perso -> health;
                        free(perso);
                        map = mapBoss;
                        perso = create_perso(map);
                        perso -> health = health;
                        lastGravityChange = currentTime1;
                        lastProjectileLoad = currentTime1;
                        lastBossMoveTime = currentTime1;
                    } else if (e.key.keysym.sym == SDLK_p && !afficherImage) {
                        perso -> health = 0;
                    }
                    }   
                    if (retourMenu) {
                        perso -> health = 0;
                        goto again;
                    } 

                if (isBossMap && (perso->x > 26.7 || perso->y > 14.94 || perso->x < 1.3 || perso->y < 1)) {
                    perso->health = 0;
                    goto again;
                } 
                    

                if (perso-> health > 0) {
                    game(boss, map, perso, state, sounds);
                    if (drawBackground(renderer, bgTextures, 5, map)) {
                        printf("Error drawing the background");
                        exit(-1);
                    }
                    if (drawMap(renderer, map, tileTextures)) {
                        printf("Error drawing the map");
                        exit(-1);
                    }
                    if (display_perso(renderer, perso, map, persoTexture, texturePortail, 0, sounds)) {
                        printf("Error drawing the perso");
                        exit(-1);
                    }
                    if (!isBossMap && !afficherImage && !parametre) {
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


                        render_text(renderer, instructionText, BLACK, &instructionBox, map);
                        //////////////////////////////* fin mouvements de chaque ennemi *////////////////////////////////////////////////
                    }
                    if (isBossMap && boss->health > 0) {
                        displayBoss(renderer, boss, map,bossTexture,0);
                        if (showAttentionImage) {
                            renderImage(renderer,"./asset/UI/attention.png",(WINWIDTH / 2 - ImageAttentionWidth / 2),(WINHEIGHT / 2 - ImageAttentionHeight / 2), ImageAttentionWidth, ImageAttentionHeight);
                        }
                        renderProjectiles(renderer,map);
                        if (!afficherImage && !parametre) {
                            renderStatusBoss(renderer,boss);
                        }
                    }
                    if (isBossMap && boss->health <= 0) {
                        perso->health = 0;
                        goto again;
                    }
                    if (!afficherImage && !parametre && !gameplay2) {
                        renderStatusHealth(renderer,perso); 
                    }

                    if (perso->health == 0) {
                        gameOver(renderer, bgTextures, 5, map, '?');
                    }
                } else {
                    gameplay2 = true;
                    if (firstSwicthMusic) {
                        Mix_HaltMusic();
                        firstSwicthMusic = !firstSwicthMusic;
                        playMusic(gMusic2);
                    }
                    isBossMap = false;
                    animateBackground(renderer, bgDeathAnim);
                    game2(renderer, playerInFight, bossDeath, attack1, attack2, attack3, attack4, attack5, attack6,scratchAnim,hitpointAnim);
                    renderStatusHealthFight(renderer,playerInFight);
                    invincibility(playerInFight);
                    if (bossDeath->health <= 0) {
                        gameplay2 = false;
                        firstSwicthMusic = !firstSwicthMusic;
                        revive(perso);
                        firstSwicthMusic = true;
                        if (firstSwicthMusic) {
                            Mix_HaltMusic();
                            firstSwicthMusic = !firstSwicthMusic;
                            playMusic(gMusic1);
                        }
                        bossDeath->health = 9;
                        playerInFight->health = 9;
                        bossDeath->phase = 1;
                        resetGameplay2(bossDeath, attack1, attack2, attack3, attack4, attack5, attack6);
                    }
                    if (playerInFight -> health == 0) {
                        firstSwicthMusic = !firstSwicthMusic;
                        gameplay2 = false;
                        bossDeath->health = 9;
                        playerInFight->health = 9;
                        bossDeath->phase = 1;
                        gameOver(renderer, bgTextures, 5, map, '!');
                        resetGameplay2(bossDeath, attack1, attack2, attack3, attack4, attack5, attack6);
                        goto again;
                    }
                }
                drawMapMenu(renderer);

                

                SDL_RenderPresent(renderer);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                Uint64 end = SDL_GetTicks();
                float elapsedMS = (end - start);
                SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
            }
        }
    }
    destroy_graph(graph, map);
    quitSDL(&renderer, &window, perso, map2, mapBoss, boss);
	free(attack1);
	free(attack2);
	free(attack3);
	free(attack4);
	free(attack5);
	free(attack6);
    free(playerInFight);
	free(bossDeath);
    free(scratchAnim);
    free(hitpointAnim);
    free(bgDeathAnim);
    free(checkpointList->xPositions);
    free(checkpointList);
    cleanupProjectiles();
    /* closeSDL_mixer(); */
    atexit(SDL_Quit);
    return 0;
}
