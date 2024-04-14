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
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "mc.h"
#include "const.h"
#include "main.h"
#include "map.h"
#include <stdbool.h>

int distance =50;
int y_position = 100;
SDL_Texture *bgTextures[6];

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError()) ;
		exit(-1) ;
	}


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

	const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255} ;
	const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255} ;

	Map *map =init_map("map1/data.txt") ;
	


	SDL_Event event ;
	int running = 1 ;

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
		if (detect_collision(map, distance, y_position + GRAVITY)) {
			y_position = (y_position / PIX_RECT) * PIX_RECT;
		} else {
			y_position += GRAVITY;
		}

		if (spriteMovement(renderer, event, &distance, &y_position, "./asset/spritesheet/ss_mc.png", map)) {
			printf("Error rendering sprite") ;
			exit(-1) ;
		}


		SDL_RenderPresent(renderer) ;

		Uint64 end = SDL_GetTicks() ;
		float elapsedMS = (end - start) ;
		SDL_Delay(fmaxf((16.666f - elapsedMS)/1.0f, 0)) ;
	}


	atexit(SDL_Quit) ;
	free(map) ;
	return 0 ;
}
