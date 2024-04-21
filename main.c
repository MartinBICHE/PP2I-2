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
#include "scroll.h"
#include "checkpoints.h"
#include <stdbool.h>

int distance = 0;
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
    TTF_Init();
    TTF_Font *font1 = TTF_OpenFont("DisposableDroidBB.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("DisposableDroidBB_bld.ttf", 20);
    TTF_Font *font3 = TTF_OpenFont("ChopinScript.ttf", 23);
        SDL_Rect dst_rect1 = {460, 480, 64, 0};
    SDL_Rect src_rect1 = {0, 0, 64, 0}; 


        SDL_Surface *surfacePapirus = IMG_Load("scroll.png");
    SDL_Texture *texturePapirus = SDL_CreateTextureFromSurface(renderer, surfacePapirus);
    SDL_Rect dst_rectScroll = {460, 280, 250, 10};
    SDL_Rect src_rectScroll = {0, 0, 250, 10}; 
    const char *text = "Hello darkness my old friend, i've come to talk with you again";


    Perso *perso3 = malloc(sizeof(Perso));
    perso3->x = 45;
    perso3->y = 3;
    perso3->vx = 0;
    perso3->vy = 0;
    /* savePosition("temp.sav", perso3); */
    CheckpointList *checkpointList = malloc(sizeof(CheckpointList));
    initCheckpointList(checkpointList);
    addCheckpoint(checkpointList, 46);
    addCheckpoint(checkpointList, 30);
    addCheckpoint(checkpointList, 20);
    addCheckpoint(checkpointList, 10);





	const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
	// const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
	const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

	Map *map = initMap("map1/data.txt");
	Perso *perso = create_perso(map);
    loadPosition("temp.sav", perso);

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
			}
		}

		perso->vx = 0;
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_A]) perso->vx -= MOOVSPEED;
		if (state[SDL_SCANCODE_D]) perso->vx += MOOVSPEED;

		updatePerso(perso, map);
		x_cam = updateCam(perso->x*PIX_RECT, x_cam);
        checkCheckpoint(checkpointList, perso, "temp.sav");

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
		}

		// SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a); // !!! seulement pour les tests de caméra (à changer)
		// SDL_Rect rect1 = {.x = x_perso*PIX_RECT - 10 - x_cam, .y = 3*PIX_RECT - 10, .w = 20, .h = 20}; // !!! seulement pour les tests de caméra (à changer)
		// SDL_RenderDrawRect(renderer, &rect1); // !!! seulement pour les tests de caméra (à changer)
		// SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a); // !!! seulement pour les tests de caméra (à changer)
		// SDL_Rect rect2 = {.x = x_perso*PIX_RECT - 9 - x_cam, .y = 3*PIX_RECT - 9, .w = 18, .h = 18}; // !!! seulement pour les tests de caméra (à changer)
    	// SDL_RenderDrawRect(renderer, &rect2); // !!! seulement pour les tests de caméra (à changer)


        /* enemy1_movement_2(renderer, texture, &src_rect, &dst_rect, &b); */
        /* enemy1_movement(renderer, texture, &src_rect1, &dst_rect1, &c); */
        /* render_text(renderer, font1, text, 0, 0, BLACK, &dst_rect1, texturePapirus, font2); */

        scroll_movement(renderer, texturePapirus, &src_rectScroll, &dst_rectScroll, font3, text, BLACK);

		SDL_RenderPresent(renderer);

		Uint64 end = SDL_GetTicks();
		float elapsedMS = (end - start);
		SDL_Delay(fmaxf((1000*DT - elapsedMS)/1.0f, 0));
	}

	
	SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font1);
    TTF_CloseFont(font2);
    TTF_CloseFont(font3);
	SDL_DestroyWindow(window);
	free(map);
	atexit(SDL_Quit) ;
    free(checkpointList->xPositions);
    free(checkpointList);
    TTF_Quit();

	return 0;
}
