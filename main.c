#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "const.h"
#include "mc.h"
#include "main.h"
#include "map.h"
#include "menu.h"

int distance = 0;
SDL_Texture *bgTextures[6];
bool showMenu = true;
bool parametre = false;
bool afficherImage = false;

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError());
        exit(-1);
    }

    const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
    const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};

    Map *map = NULL;

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

        // Si le temps écoulé est supérieur à 10000 ms (3 secondes), le chargement est complet
        if (SDL_GetTicks() - startTime >= 2000) {
            loadingComplete = true;
        }
    }

    // Fermer la fenêtre de chargement
    closeLoadingWindow();

again :
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

            // Charger les éléments du jeu
            map = init_map("map1/data.txt");
            if (!map) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors du chargement de la carte.");
                return -1;
            }

            loadBackgroundTextures(renderer, bgTextures);
            
            bool musicToggled = false;
            while (running) {

                Uint64 start = SDL_GetTicks();

                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                    } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
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
                                } else if (parametre = true) {
                                    parametre = !parametre;
                                    afficherImage = !afficherImage;
                                    // Pour l'instant l'image retour arrière ne marche pas 
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
                    }
                }

                if (retourMenu) {
                    closeMapWindow(window,renderer);
                    goto again;
                } 

                // Effacer l'écran
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                if (SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color : %s", SDL_GetError());
                    exit(-1);
                }
                if (SDL_RenderClear(renderer)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear : %s", SDL_GetError());
                    exit(-1);
                }
                if (SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color : %s", SDL_GetError());
                    exit(-1);
                }
                for (int i = 0; i < 6; ++i) {
                    if (SDL_RenderCopy(renderer, bgTextures[i], NULL, NULL)) {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error rendering background texture %d: %s", i + 1, SDL_GetError());
                        exit(-1);
                    }
                }

                if (draw_map(renderer, map, "./asset/tileset/ground-1.png")) {
                    printf("Error drawing the map");
                    exit(-1);
                }

                drawMap();

                SDL_RenderPresent(renderer);

                Uint64 end = SDL_GetTicks();
                float elapsedMS = (end - start);
                SDL_Delay(fmaxf((16.666f - elapsedMS) / 1.0f, 0));
                
            }
        }
    }

    // Nettoyer et quitter SDL_mixer
    closeSDL_mixer();

    atexit(SDL_Quit);
    free(map);
    return 0;
}
