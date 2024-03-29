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
#include <stdbool.h>

int distance =0;
SDL_Texture *bgTextures[6];

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError()) ;
		exit(-1) ;
	}

	const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255} ;
	const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255} ;

	Map *map =NULL ;

	SDL_Event event ;
	int running = 1 ;

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
        if (SDL_GetTicks() - startTime >= 10000) {
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

    // Initialiser la fenêtre de menu
    if (!initMenuWindow()) {
        SDL_Log("Erreur lors de l'initialisation de la fenêtre de menu.");
        return 1;
    }

	bool startGame = false;
	SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Boucle principale du menu
    bool quit = false;
	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				// Vérifier si le clic de souris est dans la zone du bouton "Play"
				if (e.button.button == SDL_BUTTON_LEFT) {
					int mouseX = e.button.x;
					int mouseY = e.button.y;
					if (mouseX >= ((MENU_WINDOW_WIDTH - Image1Width) / 2) && mouseX <= ((MENU_WINDOW_WIDTH - Image1Width) / 2) + Image1Width &&
						mouseY >= ((MENU_WINDOW_HEIGHT - Image1Height) / 2) && mouseY <= ((MENU_WINDOW_HEIGHT - Image1Height) / 2) + Image1Height) {
						
						// Fermer la fenêtre de menu
    					closeMenuWindow();
						startGame = true;
                    }
                }
            }
        }

		// Dessiner le menu
		drawMenu(Image1Width,Image1Height,Image2Width,Image2Height);

		// Peut-être d'autres interactions avec le menu ici

		// Mettre une pause courte pour éviter une utilisation excessive du processeur
		SDL_Delay(10);
	
		if (startGame) {
			
			// Créer la fenêtre de jeu et le renderer
			SDL_Window *window ;
			window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINWIDTH, WINHEIGHT, SDL_WINDOW_SHOWN) ;
			if (!window) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init : %s", SDL_GetError()) ;
				exit(-1) ;
			}

			SDL_Renderer *renderer ;
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) ;
			if (!renderer) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init : %s", SDL_GetError()) ;
				exit(-1) ;
			}

			// Charger les éléments du jeu
			map = init_map("map1/data.txt");
			if (!map) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur lors du chargement de la carte.");
				return -1;
			}

			loadBackgroundTextures(renderer, bgTextures);
			
			
			while (running) {

				Uint64 start = SDL_GetTicks() ;

				if (SDL_PollEvent(&event)) {
					switch(event.type) {
					case SDL_QUIT :
						running = 0 ;
						break ;
					case SDL_KEYUP :
						break ;
					}
				}

				if (SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a)) {
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color : %s", SDL_GetError()) ;
					exit(-1) ;
				}
				if (SDL_RenderClear(renderer)) {
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render clear : %s", SDL_GetError()) ;
					exit(-1) ;
				}
				if (SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a)) {
					SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color : %s", SDL_GetError()) ;
					exit(-1) ;
				}
				for (int i = 0; i < 6; ++i) {
					if (SDL_RenderCopy(renderer, bgTextures[i], NULL, NULL)) {
						SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error rendering background texture %d: %s", i + 1, SDL_GetError());
						exit(-1);
					}
				}

				
				if (draw_map(renderer, map, "./asset/tileset/ground-1.png")) {
					printf("Error drawing the map") ;
					exit(-1) ;
				}

				// if (displayBackground(renderer,"./asset/background/Foret/plan-6.png")) {
				// 	printf("Error drawing the background") ;
				// 	exit(-1) ;
				// }
				// if (displayBackground(renderer,"./asset/background/Foret/plan-5.png")) {
				// 	printf("Error drawing the background") ;
				// 	exit(-1) ;
				// }
				// if (displayBackground(renderer,"./asset/background/Foret/plan-4.png")) {
				// 	printf("Error drawing the background") ;
				// 	exit(-1) ;
				// }
				// if (displayBackground(renderer,"./asset/background/Foret/plan-3.png")) {
				// 	printf("Error drawing the background") ;
				// 	exit(-1) ;
				// }
				// if (displayBackground(renderer,"./asset/background/Foret/plan-2.png")) {
				// 	printf("Error drawing the background") ;
				// 	exit(-1) ;
				// }
				// if (displayBackground(renderer,"./asset/background/Foret/plan-1.png")) {
				// 	printf("Error drawing the background") ;
				// 	exit(-1) ;
				// }
				// if (spriteMovement(renderer, event, &distance, "./asset/spritesheet/ss_mc.png")) {
				// 	printf("Error rendering sprite") ;
				// 	exit(-1) ;
				// }


				SDL_RenderPresent(renderer) ;

				Uint64 end = SDL_GetTicks() ;
				float elapsedMS = (end - start) ;
				SDL_Delay(fmaxf((16.666f - elapsedMS)/1.0f, 0)) ;
			}
		}
	}
	
    // Nettoyer et quitter SDL_mixer
    closeSDL_mixer();

	atexit(SDL_Quit) ;
	free(map) ;
	return 0 ;
}
