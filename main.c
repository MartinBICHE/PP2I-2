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

    EnemyStateData enemyStateData;
    initEnemy1(600, 660, &enemyStateData);



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
        resetGame(&window, &renderer, &map2, &mapBoss, &perso, &boss);
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

            loadBackgroundTextures(renderer, bgTextures, 5);
            loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
            loadPersoTexture(renderer, &persoTexture, "./asset/spritesheet/ss_mc.png");
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
                        free(perso);
                        map = mapBoss;
                        perso = create_perso(map);
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
                    if (display_perso(renderer, perso, map, persoTexture, texturePortail, 0, sounds)) {
                        printf("Error drawing the perso");
                        exit(-1);
                    }
                    if (!isBossMap) {
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
                        gameOver(renderer, bgTextures, 5, map, '?');
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
