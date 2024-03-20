#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
// #include <SDL2/SDL2_gfxPrimitives.h>
// #include <SDL2/SDL_image.h>


const int height = 8 ;
const int width = 48 ;
char map[height][width] ;
const int pix_rect = 30 ;


void init_map(char *name) {
	FILE *f = fopen(name, "r") ;
    // char buffer[100];
    for (int i = 0 ; i < height ; i++) {
        fgets(map[i], 49, f) ;
	}
    fclose(f) ;
}


int draw_map (SDL_Renderer *renderer) {
	for (int i = 0 ; i < height ; i++) {
		for (int j = 0 ; j < width ; j++) {
			if (map[i][j] == '1') {
				const SDL_Rect rect = {.x = j*pix_rect+1, .y = i*pix_rect+1, .w = pix_rect-2, .h = pix_rect-2} ;
				SDL_RenderDrawRect(renderer, &rect) ;
			}
		}
	}
	
	return 0 ;
}




int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init : %s", SDL_GetError()) ;
		exit(-1) ;
	}

	

	int const WINWIDTH = width*pix_rect ;
	int const WINHEIGHT = height*pix_rect ;
	SDL_Color const BACKGROUD_COLOR = {.r = 0, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE} ;
	SDL_Color const WHITE = {.r = 255, .g = 255, .b = 255, .a = 255} ;

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

	init_map("map1/data.txt") ;
	

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

		if (SDL_SetRenderDrawColor(renderer, BACKGROUD_COLOR.r, BACKGROUD_COLOR.g, BACKGROUD_COLOR.b, BACKGROUD_COLOR.a)) {
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
		if (draw_map(renderer)) {
			printf("Error drawing the map") ;
			exit(-1) ;
		}
		SDL_RenderPresent(renderer) ;


	}


	atexit(SDL_Quit) ;
	return 0 ;
}
