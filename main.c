#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "init.h"
#include "map.h"
#include "mapBoss.h"
#include "perso.h"
#include "scroll.h"
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
#include "pendule.h"
#include "textures.h"
#include "fonts.h"
#include "health.h"
#include "enemyFleche.h"
#include "enemyBat.h"
#include "fight.h"
#include "menu.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;
SDL_Texture *bgTextures[6];
bool showMenu = true;
bool parametre = false;
bool afficherImage = false;
bool retourMenu = false;
bool startGame = false;
bool prevShowMenu = true;
bool musicToggled = false;
bool quit = false;
bool running = true;
SDL_Texture *tileTextures;


int main(int argc, char **argv) {
    initSDL(&window, &renderer);

	const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
	const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
	const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

	Perso *playerInFight = (Perso*)malloc(sizeof(Perso));
	playerInFight->y = QUARTERHEIGHT-SPRITESIZE/2;
	playerInFight->x = TIERWIDTH/2-SPRITESIZE/2;
	Map *map = initMap("map1/data.txt");
	Perso *perso = create_perso(map);

	float x_cam = 0; // cam à gauche au début

	SDL_Event event;
	int running = 1;

	loadBackgroundTextures(renderer, bgTextures, 5);
	loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");

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
    playMusic();

again :
    retourMenu = false;
    startGame = false;
    prevShowMenu = true;

    // Boucle principale du menu
    quit = false;

    while (!quit) {
        interactionMenu();   

        if (startGame) {
            if (!map) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors du chargement de la carte.");
                return -1;
            }

            Perso *perso = create_perso(map);

            float x_cam = 0; // cam à gauche au début
            
            float x_perso = 2; // !!! seulement pour les tests de caméra (à changer) x_perso est en nombre de tiles et pas en pixels

            loadBackgroundTextures(renderer, bgTextures, 5);
            loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");

            while (running) {
                Uint64 start = SDL_GetTicks();
                while (SDL_PollEvent(&e) != 0) {
                    interactionPauseJeu();
                
                    if (e.key.keysym.sym == SDLK_SPACE && afficherImage == false) {
					    jump(perso, map);}
				}

                if (retourMenu) {
                    goto again;
                } 

                perso->vx = 0;
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                if (state[SDL_SCANCODE_A]) perso->vx -= MOVSPEED;
                if (state[SDL_SCANCODE_D]) perso->vx += MOVSPEED;


                updatePerso(perso, map);
                x_cam = updateCam(perso->x*PIX_RECT, x_cam);

                if (drawBackground(renderer, bgTextures, 5, x_cam)) {
                    printf("Error drawing the background");
                    exit(-1);
                }
                if (drawMap(renderer, map, "./asset/tileset/ground-1.png", x_cam, tileTextures)) {
                    printf("Error drawing the map");
                    exit(-1);
                }
                // if (fightMovement(renderer, event, playerInFight)) {
                // 	printf("Error drawing the fight");
                // 	exit(-1);
                // }

                SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
                if (display_perso(renderer, perso, x_cam)) {
                    printf("Error drawing the perso");
                    exit(-1);
                }

                drawMapMenu();

                // SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a); // !!! seulement pour les tests de caméra (à changer)
                // SDL_Rect rect1 = {.x = x_perso*PIX_RECT - 10 - x_cam, .y = 3*PIX_RECT - 10, .w = 20, .h = 20}; // !!! seulement pour les tests de caméra (à changer)
                // SDL_RenderDrawRect(renderer, &rect1); // !!! seulement pour les tests de caméra (à changer)
                // SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a); // !!! seulement pour les tests de caméra (à changer)
                // SDL_Rect rect2 = {.x = x_perso*PIX_RECT - 9 - x_cam, .y = 3*PIX_RECT - 9, .w = 18, .h = 18}; // !!! seulement pour les tests de caméra (à changer)
                // SDL_RenderDrawRect(renderer, &rect2); // !!! seulement pour les tests de caméra (à changer)

                SDL_RenderPresent(renderer);

                Uint64 end = SDL_GetTicks();
                float elapsedMS = (end - start);
                SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
            }
        }
    }
    quitSDL(&renderer, &window, perso, map);
    closeSDL_mixer();
    atexit(SDL_Quit);
    return 0;
}
