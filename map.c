#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "const.h"


Map *init_map(char *name) {
	Map *res = malloc(sizeof(Map)) ;
	FILE *f = fopen(name, "r") ;
    // char buffer[100];
    for (int i = 0 ; i < HEIGHT ; i++) {
        fgets(res->matrix[i], 49, f) ;
	}
    fclose(f) ;
	res->start_x = 4.0*PIX_RECT ;
	res->start_y = 4.0*PIX_RECT ;
	return res ;
}


int draw_map(SDL_Renderer *renderer, Map *map) {
	for (int i = 0 ; i < HEIGHT ; i++) {
		for (int j = 0 ; j < WIDTH ; j++) {
			if (map->matrix[i][j] == '1') {
				const SDL_Rect rect = {.x = j*PIX_RECT+1, .y = i*PIX_RECT+1, .w = PIX_RECT-2, .h = PIX_RECT-2} ;
				SDL_RenderDrawRect(renderer, &rect) ;
			}
		}
	}
	
	return 0 ;
}

