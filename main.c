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
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "dialog_box.h"
#include "display.h"
#include <stdbool.h>

SDL_Texture *bgTextures[6];


int main(int argc, char **argv) {
    int b = 1;


    Enemy2 *enemy4 = malloc(sizeof(Enemy2));
    enemy4->image_path = "bubble.png";
    enemy4->speed = 45;
    enemy4->xPosition = 1*PIX_RECT;
    enemy4->yPosition = 4*PIX_RECT ;
    enemy4->health = 0;
    enemy4->collision_max_x = WIDTH*PIX_RECT;
    enemy4->collision_min_x = 1*PIX_RECT;
    enemy4->yInitialPosition = 4;

    Enemy3 *enemy3 = malloc(sizeof(Enemy3));
    enemy3->image_path = "bubble.png";
    enemy3->speed = 45;
    enemy3->xPosition = 1*PIX_RECT;
    enemy3->yPosition = 4*PIX_RECT ;
    enemy3->xCollisionMax = 5*PIX_RECT;
    enemy3->xCollisionMin = 1*PIX_RECT;


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
    SDL_Surface *surface = IMG_Load("ennemy1_2.png");
    SDL_Surface *surface_2 = IMG_Load("bubble.png");
    SDL_Surface *surfaceBox = IMG_Load("papirus.png");
    SDL_Surface *surfaceBoxName = IMG_Load("transparent.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Texture *texture_2 = SDL_CreateTextureFromSurface(renderer, surface_2);
    SDL_Texture *textureBox = SDL_CreateTextureFromSurface(renderer, surfaceBox);
    SDL_Texture *textureBoxName = SDL_CreateTextureFromSurface(renderer, surfaceBoxName);
    SDL_Rect dest_rect = {460, 230, 64, 0};
    SDL_Rect src_rect = {0, 0, 64, 0};
    TTF_Init();
    TTF_Font *font1 = TTF_OpenFont("DisposableDroidBB.ttf", 20);
    TTF_Font *font2 = TTF_OpenFont("DisposableDroidBB_bld.ttf", 20);
    MoveList *list_mov_right = rightMovementList(1, 4, *map, WIDTH-1);
    MoveList *list_mov_left = leftMovementList(1, 4, *map, WIDTH-4);
    MoveList *list_left = leftMovementList(1, 4, *map, WIDTH-4);
    concatenate(&list_mov_right, &list_mov_left);

    const char *text = "         a imaginer que c'est interresant ce que ca raconte";
    const char *textName = "Jannik:";
    SDL_Color BEIGE = {200, 172, 127, 255};
    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font1, text, BLACK, 200);
    SDL_Surface *textSurfaceName = TTF_RenderText_Blended_Wrapped(font2, textName, BLACK, 200);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Texture *textTextureName = SDL_CreateTextureFromSurface(renderer, textSurfaceName);
    
    int textureWidth, textureHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textureWidth, &textureHeight);
    


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

        enemy1_movement(renderer, texture, &src_rect, &dest_rect, &b);
        enemy2_movement(renderer, texture_2, enemy4, 1, map, &list_mov_right);
        enemy3_movement(renderer, texture_2, enemy3, 1);




        render_box(renderer, textureBox, textTexture, 10, 10);
        render_box(renderer, textureBoxName, textTextureName, 10, 10);





		SDL_RenderPresent(renderer) ;

		Uint64 end = SDL_GetTicks() ;
		float elapsedMS = (end - start) ;
		SDL_Delay(fmaxf((16.666f - elapsedMS)/1.0f, 0)) ;
	}

	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(map);
    free(surface);
    free(surface_2);
    free(textSurface);
    free(textSurfaceName);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_2);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(textTextureName);
    TTF_CloseFont(font1);
	atexit(SDL_Quit) ;

    /* on pourra faire un fonction SDL_Close par exemple qui aura SDL_Destroy window texture etc, idem pour init */

	return 0 ;
}
