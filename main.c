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
SDL_Texture *scratchTexture;
SDL_Texture *hitpointTexture;
SDL_Texture *bgDeathTexture;
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

    loadDeathTexture(renderer, &scratchTexture, "./asset/spritesheet/scratch.png");
    loadDeathTexture(renderer, &hitpointTexture, "./asset/spritesheet/hitpoint.png");
    loadDeathTexture(renderer, &bgDeathTexture, "./asset/background/satan.png");


	AttackFight *attack1 = initAttack(0, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack2 = initAttack(TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack3 = initAttack(2*TIERWIDTH, 2*QUARTERHEIGHT, bossDeath);
	AttackFight *attack4 = initAttack(0, 0, bossDeath);
	AttackFight *attack5 = initAttack(TIERWIDTH, 0, bossDeath);
	AttackFight *attack6 = initAttack(2*TIERWIDTH, 0, bossDeath);

    Animation *scratchAnim = malloc(sizeof(Animation));
    Animation *hitpointAnim = malloc(sizeof(Animation));
    Animation *bgDeathAnim = malloc(sizeof(Animation));


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
    /* Node **graph = create_graph(map); */

    EnemyStateData enemyStateData;
    initEnemy1(119*map->pix_rect, 15*map->pix_rect, &enemyStateData);

    Enemy3 enemy3;
    INIT_ENEMY3(&enemy3, map, 14, 14, 23);

    Enemy2 enemy2;
    /* Node *goalEnemy2 = &graph[7][6]; */
    /* Node *startEnemy2 = &graph[7][30]; */
    /* initEnemy2(&enemy2, startEnemy2, goalEnemy2, map); */


    EnemyPenduleData enemyPenduleData;
    initEnemyPendule(&enemyPenduleData, 123*map->pix_rect, 1*map->pix_rect);


    EnemyBatData enemyBatData;
    /* initEnemyBat(&enemyBatData, 56*map->pix_rect, 4*map->pix_rect, 61*map->pix_rect, startEnemy2, goalEnemy2, map); */  

    EnemyFlecheData enemyFlecheData;
    /* initEnemyFleche(&enemyFlecheData, 100, 300); */
    initEnemyFleche(&enemyFlecheData, 48*map->pix_rect, 14*map->pix_rect);

    ProjectileData projectile;
    initProjectile(100, 100, &projectile);


    AttackData attack;
    initAttackAnimation(200, 100, &attack);


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
            initAnimation(scratchAnim, scratchTexture, 2400, 160, 10, 150);
            initAnimation(hitpointAnim, hitpointTexture, 960 ,160, 4, 200);
            initAnimation(bgDeathAnim, bgDeathTexture, 12960,480, 18, 100);



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
                        /* enemy2_follow(renderer, &enemy2, graph, map); */
                        updatePersoEnemy2(perso, map, &enemy2);
                        enemy2Attack(&enemy2, perso, map);
                        enemy3_movement(renderer, &enemy3, map);
                        updatePersoEnemy3(perso, map, &enemy3);
                        enemy3Attack(&enemy3, perso, map);
                        enemyBat_mouvement(renderer, &enemyBatData, map, perso);
                        /* updatePersoEnemyBat(perso, map, &enemyBatData); fontionne pas très bien, à voir*/
                        batAttack(&enemyBatData, perso, map);
                        enemyFleche_mouvement(renderer, &enemyFlecheData, map);
                        updatePersoEnemyFleche(perso, map, &enemyFlecheData);
                        flecheAttack(&enemyFlecheData, perso, map);
                        enemyPendule_mouvement(renderer, &enemyPenduleData, map);
                        penduleAttack(&enemyPenduleData, perso, map);
                        SDL_Rect dst_rect = {10, 10, 16, 32};
                        SDL_Rect src_rect = {0, 0, 16, 32};
                        SDL_RenderCopy(renderer, textureAttack, &src_rect, &dst_rect);
                        projectile_mouvement(renderer, &projectile, map);
                        attack_mouvement(renderer, &attack, map);
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
                    animateBackground(renderer, bgDeathAnim);
                    game2(renderer, playerInFight, bossDeath, attack1, attack2, attack3, attack4, attack5, attack6,scratchAnim,hitpointAnim);
                    renderStatusHealthFight(renderer,playerInFight);
                    invincibility(playerInFight);
                    if (bossDeath->health <= 0) {
                        revive(perso);
                        bossDeath->health = 9;
                        playerInFight->health = 9;
                        bossDeath->phase = 1;
                        resetGameplay2(bossDeath, attack1, attack2, attack3, attack4, attack5, attack6);
                    }
                    if (playerInFight -> health == 0) {
                        bossDeath->health = 9;
                        playerInFight->health = 9;
                        bossDeath->phase = 1;
                        resetGameplay2(bossDeath, attack1, attack2, attack3, attack4, attack5, attack6);
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
    free(boss);
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
