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
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;
SDL_Texture *bgTextures[6];
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

	Perso *playerInFight = (Perso*)malloc(sizeof(Perso));
	playerInFight->y = QUARTERHEIGHT-SPRITESIZE/2;
	playerInFight->x = TIERWIDTH/2-SPRITESIZE/2;
    Map *map = initMap("map2");
	Perso *perso = create_perso(map);
    Boss *boss = NULL;

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int running = 1;

    loadBackgroundTextures(renderer, bgTextures, 5);
    loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
    loadPersoTexture(renderer, &persoTexture, "./asset/spritesheet/ss_mc.png");

    EnemyStateData enemyStateData;
    initEnemy1(300, 460, &enemyStateData);

    // Initialiser SDL_mixer
    if (!initSDL_mixer()) {
        SDL_Log("Erreur lors de l'initialisation de SDL_mixer.");
        return 1;
    }

    // Charger la musique
    if (!loadMusic()) {
        SDL_Log("Erreur lors du chargement de la musique.");
        closeSDL_mixer();
        return 1;
    }

    // Jouer la musique lorsque le menu s'ouvre
    // playMusic();

again :

    retourMenu = false;
    startGame = false;
    prevShowMenu = true;

    resetGame(&window, &renderer, &map, &perso, &boss);
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
                        map = initMap("mapBoss1");
                        perso = create_perso(map);
                        boss = create_boss(map);
                        lastGravityChange = currentTime1;
                        lastProjectileLoad = currentTime1;
                        lastBossMoveTime = currentTime1;
                    }

                    } if (retourMenu) {
                        goto again;
                    } 

                    game(enemyStateData,boss,map,perso,state);

                // x_cam = updateCamm(perso->x*PIX_RECT, x_cam);

                if (drawBackground(renderer, bgTextures, 5, map)) {
                    printf("Error drawing the background");
                    exit(-1);
                }
                if (drawMap(renderer, map, tileTextures)) {
                    printf("Error drawing the map");
                    exit(-1);
                }
                // if (fightMovement(renderer, event, playerInFight)) {
                // 	printf("Error drawing the fight");
                // 	exit(-1);
                // }
                if (display_perso(renderer, perso, map, persoTexture, 0)) {
                    printf("Error drawing the perso");
                    exit(-1);
                }
                if (!isBossMap) {
                    enemy1_movement(renderer, &enemyStateData, map);
                }
                if (isBossMap) {
                    displayBoss(renderer, boss, map);
                    if (showAttentionImage) {
                        renderImage(renderer,"./asset/UI/attention.png",(WINWIDTH / 2 - ImageAttentionWidth / 2),(WINHEIGHT / 2 - ImageAttentionHeight / 2), ImageAttentionWidth, ImageAttentionHeight);
                    }
                    renderProjectiles(renderer);
                }
                drawMapMenu(renderer);
                    
                // SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a); // !!! seulement pour les tests de caméra (à changer)
                // SDL_Rect rect1 = {.x = x_perso*PIX_RECT - 10 - x_cam, .y = 3*PIX_RECT - 10, .w = 20, .h = 20}; // !!! seulement pour les tests de caméra (à changer)
                // SDL_RenderDrawRect(renderer, &rect1); // !!! seulement pour les tests de caméra (à changer)
                // SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a); // !!! seulement pour les tests de caméra (à changer)
                // SDL_Rect rect2 = {.x = x_perso*PIX_RECT - 9 - x_cam, .y = 3*PIX_RECT - 9, .w = 18, .h = 18}; // !!! seulement pour les tests de caméra (à changer)
                // SDL_RenderDrawRect(renderer, &rect2); // !!! seulement pour les tests de caméra (à changer)

                renderStatusHealth(renderer,perso);

                SDL_RenderPresent(renderer);

                Uint64 end = SDL_GetTicks();
                float elapsedMS = (end - start);
                SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
            
            }
        }
    }
    cleanupProjectiles();
    quitSDL(&renderer, &window, perso, map);
    closeSDL_mixer();
    free(playerInFight); // à bouger ultérieurment dans init.c
    atexit(SDL_Quit);
    return 0;
}
