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
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "const.h"
#include "main.h"
#include "map.h"
#include "perso.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "dialog_box.h"
#include "display.h"
#include "enemy_interactions.h"
#include <stdbool.h>


int distance =50;
int y_position = 100;
SDL_Texture *bgTextures[6];


int main(int argc, char **argv) {

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError());
		exit(-1);
	}


	SDL_Window *window;
	window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINWIDTH, WINHEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init : %s", SDL_GetError());
		exit(-1);
	}

	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init : %s", SDL_GetError());
		exit(-1);
	}

	// const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
	// const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
	const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

	Map *map = initMap("map1/data.txt");
	Perso *perso = create_perso(map);

	float x_cam = 0; // cam à gauche au début


	SDL_Event event;
	int running = 1;

	loadBackgroundTextures(renderer, bgTextures, 5);

	while (running) {

		Uint64 start = SDL_GetTicks();

		if (SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT :
				running = 0;
				break;
			case SDL_KEYDOWN :
				if (event.key.keysym.sym == SDLK_SPACE) {
					jump(perso, map);
				}
				// Vérifier si la touche Entrée est pressée
				// Bloc de code ci-dessous à décommenter quand les ennemis seront ajoutés au code principal
				/* if (event.key.keysym.sym == SDLK_RETURN) {
					// Gérer l'attaque du personnage
					handleCharacterAttack(perso, enemy); // enemy à définir quand les ennemis seront ajoutés au code principal
				} */
			}
		}

		perso->vx = 0;
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_A]) perso->vx -= MOOVSPEED;
		if (state[SDL_SCANCODE_D]) perso->vx += MOOVSPEED;

		updatePerso(perso, map, enemies, numEnemies); // Attention, cette ligne empêche le code de s'exécuter normalement car il faudra définir un tableau d'ennemis
		x_cam = updateCam(perso->x*PIX_RECT, x_cam);

		if (drawBackground(renderer, bgTextures, 5, x_cam)) {
			printf("Error drawing the background");
			exit(-1);
		}
		if (drawMap(renderer, map, "./asset/tileset/ground-1.png", x_cam)) {
			printf("Error drawing the map");
			exit(-1);
		}


		SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
		if (display_perso(renderer, perso, x_cam)) {
			printf("Error drawing the perso");
			exit(-1);
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
		if (detect_collision(map, distance, y_position + GRAVITY)) {
			y_position = (y_position / PIX_RECT) * PIX_RECT;
		} else {
			y_position += GRAVITY;
		}

		if (spriteMovement(renderer, event, &distance, &y_position, "./asset/spritesheet/ss_mc.png", map)) {
			printf("Error rendering sprite") ;
			exit(-1) ;
		}

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

	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(map);
	atexit(SDL_Quit) ;

	return 0;
}
