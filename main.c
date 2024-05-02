#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "init.h"
#include "map.h"
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
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "fight.h"
#include "perso.h"
#include "mc.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "display.h"
#include "pendule.h"
#include "textures.h"
#include "fonts.h"
#include "health.h"
#include "enemyFleche.h"
#include "enemyBat.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture *bgTextures[6];
bool showMenu = true;
bool parametre = false;
bool afficherImage = false;
SDL_Texture *tileTextures;

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError());
        exit(-1);
    }

    const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
    const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
    const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

    Map *map = initMap("map1");

    SDL_Event event;
    bool running = true;

    // Initialisation de la fenêtre de chargement
    if (!initLoadingWindow()) {
        SDL_Log("Erreur lors de l'initialisation de la fenêtre de chargement.");
        return 1;
    }

    // Boucle de chargement
    bool loadingComplete = false;
    Uint32 startTime = SDL_GetTicks();
    while (!loadingComplete) {
        // Gestion des événements de la fenêtre de chargement
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            // Si l'utilisateur ferme la fenêtre
            if (e.type == SDL_QUIT) {
                loadingComplete = true;
            }
        }


        // Dessiner le motif de chargement
        drawLoading();

        // Perso *playerInFight = (Perso*)malloc(sizeof(Perso));
        // playerInFight->y = QUARTERHEIGHT-SPRITESIZE/2;
        // playerInFight->x = TIERWIDTH/2-SPRITESIZE/2;
        // Map *map = initMap("map1/data.txt");
        // Perso *perso = create_perso(map);


        // Si le temps écoulé est supérieur à 10000 ms (3 secondes), le chargement est complet
        if (SDL_GetTicks() - startTime >= 2000) {
            loadingComplete = true;
        }
    }

    // Fermer la fenêtre de chargement
    closeLoadingWindow();

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
    // Initialiser la fenêtre de menu
    if (!initMenuWindow()) {
        SDL_Log("Erreur lors de l'initialisation de la fenêtre de menu.");
        return 1;
    }

    bool retourMenu = false;
    bool startGame = false;
    bool prevShowMenu = true;

    // Boucle principale du menu
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                    if (mouseX >= 0 && mouseX <= Image3Width && mouseY >= 0 && mouseY <= Image3Height && showMenu == true) {
                        quit = true; // Fermer toutes les fenêtres
                    } else if (mouseX >= ((MENU_WINDOW_WIDTH - Image1Width) / 2) && mouseX <= ((MENU_WINDOW_WIDTH - Image1Width) / 2) + Image1Width &&
                                mouseY >= ((MENU_WINDOW_HEIGHT - Image1Height) / 2) && mouseY <= ((MENU_WINDOW_HEIGHT - Image1Height) / 2) + Image1Height && showMenu == true) {
                        closeMenuWindow(); // Fermer la fenêtre de menu
                        startGame = true;
                        retourMenu = false;
                        showMenu = true;
                    } else if (mouseX >= (MENU_WINDOW_WIDTH - Image2Width) && mouseX <= MENU_WINDOW_WIDTH &&
                        mouseY >= 0 && mouseY <= Image2Height) {
                        showMenu = false;
                    }
            } else if (e.type == SDL_KEYUP ) {
                if (e.key.keysym.sym == SDLK_ESCAPE && showMenu == false) {
                    showMenu = true;
                }
            }
            
        }
        if (prevShowMenu != showMenu) {
            if (!showMenu) {
                toggleMusic();
            } else {
                toggleMusic();
            }
            prevShowMenu = showMenu;
        }

        // Dessiner le menu    
        drawMenu();    
        
        // Mettre une pause courte pour éviter une utilisation excessive du processeur
        SDL_Delay(10);

        if (startGame) {

            // Créer la fenêtre de jeu et le renderer
            initPlayWindow();

            if (!map) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors du chargement de la carte.");
                return -1;
            }

            Perso *perso = create_perso(map);

            float x_cam = 0; // cam à gauche au début
            
            float x_perso = 2; // !!! seulement pour les tests de caméra (à changer) x_perso est en nombre de tiles et pas en pixels

            int running = true;

            loadBackgroundTextures(renderer, bgTextures, 5);
            loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");

            bool musicToggled = false;
            while (running) {

                Uint64 start = SDL_GetTicks();

                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                    } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = e.button.x;
                        int mouseY = e.button.y;
                        
                        if (afficherImage) {
                            if (mouseX >= ((WINWIDTH - ImageQuitterJeuWidth) / 2) && mouseX <= ((WINWIDTH - ImageQuitterJeuWidth) / 2) + ImageQuitterJeuWidth &&
                                mouseY >= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) && mouseY <= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) + ImageQuitterJeuHeight) {
                                quit = true;
                                running = false; // Quitter le jeu si l'image "Quitter le jeu" est cliquée
                            }
                            else if (mouseX >= ((WINWIDTH - ImageRetourMenuWidth) / 2) && mouseX <= ((WINWIDTH - ImageRetourMenuWidth) / 2) + ImageRetourMenuWidth &&
                                mouseY >= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) && mouseY <= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) + ImageRetourMenuHeight) {
                                retourMenu = !retourMenu;
                                toggleMusic();
                                afficherImage = !afficherImage;
                            }
                            else if (mouseX >= (WINWIDTH - ImageParametrePauseWidth) && mouseX <= WINWIDTH && 
                                mouseY >= 0 && mouseY <= ImageParametrePauseHeight) {
                                parametre = !parametre;
                                afficherImage = false;
                            } else if (mouseX >= 0 && mouseX <= ImageRetourArrièreWidth &&
                                mouseY >= 0 && mouseY <= ImageRetourArrièreHeight && afficherImage == true) {
                                if (afficherImage == true) {
                                    afficherImage = !afficherImage;
                                    toggleMusic();
                                }
                            }   
                        }
                    } else if (e.type == SDL_KEYUP) {
                        if (e.key.keysym.sym == SDLK_ESCAPE && afficherImage == false && parametre == false) {
                            afficherImage = !afficherImage;
                            toggleMusic();
                            parametre = false;
                        }
                        else if (e.key.keysym.sym == SDLK_ESCAPE && afficherImage == true) {
                            afficherImage = !afficherImage;
                            toggleMusic();
                        }
                        else if (e.key.keysym.sym == SDLK_ESCAPE && parametre == true) {
                            parametre = !parametre;
                            afficherImage=!afficherImage;
                        }
                    } else if (e.key.keysym.sym == SDLK_SPACE && afficherImage == false) {
					    jump(perso, map);
				}
                }

                if (retourMenu) {
                    closeMapWindow(); 
                    goto again;
                } 

                perso->vx = 0;
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                if (state[SDL_SCANCODE_A] && afficherImage == false) perso->vx -= MOVSPEED;
                if (state[SDL_SCANCODE_D] && afficherImage == false) perso->vx += MOVSPEED;

                updatePerso(perso, map);
                x_cam = updateCam(perso->x*PIX_RECT, x_cam);

                if (drawBackground(renderer, bgTextures, 5, x_cam)) {
                    printf("Error drawing the background");
                    exit(-1);
                }
                if (drawMap(renderer, map, "./asset/tileset/ground-1.png", x_cam,tileTextures)) {
                    printf("Error drawing the map");
                    exit(-1);
                }

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
                SDL_Delay(fmaxf((1000*DT - elapsedMS)/1.0f, 0));
            }
        }
    }

    // Nettoyer et quitter SDL_mixer
    closeSDL_mixer();   
    atexit(SDL_Quit);
    free(map);
    return 0;
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(tileTextures);
	free(map);
	atexit(SDL_Quit);
    return 0;
}