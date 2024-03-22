#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "const.h"
#include "main.h"
#include "map.h"







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

	while (running) {
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
		if (draw_map(renderer, map)) {
			printf("Error drawing the map") ;
			exit(-1) ;
		}

		SDL_RenderPresent(renderer) ;


	}


	atexit(SDL_Quit) ;
	free(map) ;
	return 0 ;
}
