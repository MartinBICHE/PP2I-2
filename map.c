#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "const.h"
#include "map.h"

Map *initMap(char *name) {
	char *str_data = malloc(strlen(name) + 10);
    char *str_metadata = malloc(strlen(name) + 15);
	strcpy(str_data, name);
	strcpy(str_metadata, name);
	strcat(str_data, "/data.txt");
	strcat(str_metadata, "/metadata.txt");
	Map *res = malloc(sizeof(Map));
	FILE *f = fopen(str_data, "r");
    for (int i = 0 ; i < HEIGHT ; i++) {
        if (fgets(res->matrix[i], 49, f) == 0) {
			printf("String length is null");
		}
	}
    fclose(f);
	res->start_x = 4.0f; // en nombre de tiles
	res->start_y = 4.0f; // en nombre de tiles
	free(str_data);
	free(str_metadata);
	return res;
}


float updateCam(float x_perso, float x_cam) {
	if (x_perso - x_cam > 0.6*WINWIDTH) {
		x_cam = x_perso - 0.6*WINWIDTH;
	}
	if (x_perso - x_cam < 0.4*WINWIDTH) {
		x_cam = x_perso - 0.4*WINWIDTH;
	}
	if (x_cam < 0) {
		x_cam = 0;
	}
	if (x_cam > LEVEL_WIDTH - WINWIDTH) {
		x_cam = LEVEL_WIDTH - WINWIDTH;
	}
	return x_cam;
}

// Fonction d'affichage du background

void loadBackgroundTextures(SDL_Renderer *renderer, SDL_Texture *bgTextures[], int layer) {
    int index = layer;
	for (int i = 0; i < layer; ++i) {
        char imagePath[100];
        snprintf(imagePath, 100, "./asset/background/Foret/plan-%d.png", index-i);
        bgTextures[i] = IMG_LoadTexture(renderer, imagePath);
        if (!bgTextures[i]) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading background texture %d: %s", i + 1, SDL_GetError());
            exit(-1);
        }
    }
}

int drawBackground(SDL_Renderer *renderer, SDL_Texture *bgTextures[], int layer, float x_cam) {
	for (int i = 0; i < layer; ++i) {
		float parallax = (float)i / (layer - 1);
		int textureWidth;
        SDL_QueryTexture(bgTextures[i], NULL, NULL, &textureWidth, NULL);
		int repeats = (WINWIDTH / textureWidth) + 2;
		for (int j = 0; j < repeats; ++j) {
			int x_position = j * textureWidth - parallax * x_cam;
            if (x_position + textureWidth < 0 || x_position > WINWIDTH) continue;
			SDL_Rect bgRect = {.x = j * textureWidth - parallax * x_cam, .y = 0, .w = textureWidth, .h = WINHEIGHT};
			SDL_RendererFlip flip = (j % 2 == 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
			if (SDL_RenderCopyEx(renderer, bgTextures[i], NULL, &bgRect, 0.0, NULL, flip)) {
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error rendering background texture %d: %s", i + 1, SDL_GetError());
				exit(-1);
			}
		}
	}
	return 0;
}

// Fonction d'affichage des tiles

void loadTileTextures(SDL_Renderer *renderer, SDL_Texture **tileTexture, char *tilePath) {
	SDL_Surface *tileSurface = IMG_Load(tilePath);
	if (!tileSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in tile surface init %s", IMG_GetError());
		exit(-1);
	}
	*tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	if (!*tileTexture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in tile texture init: %s", SDL_GetError());
		exit(-1);
	}
	SDL_FreeSurface(tileSurface);
}

void display_tile(SDL_Renderer *renderer, int xoffset, int yoffset, int xpos, int ypos , float x_cam, SDL_Texture *tileTexture) {
	int tilesetWidth, tilesetHeight;
	if (SDL_QueryTexture(tileTexture,NULL, NULL, &tilesetWidth, &tilesetHeight)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in query tile texture: %s", SDL_GetError());
		exit(-1);
	}

	int tileWidth = tilesetWidth / 5;
	int tileHeight = tilesetHeight / 5;

	SDL_Rect tileRect = {.x = xoffset*tileWidth, .y = yoffset*tileHeight, .w = tileWidth, .h = tileHeight};
	SDL_Rect destRect = {.x = xpos*PIX_RECT - x_cam, .y = ypos*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};

	if (SDL_RenderCopy(renderer, tileTexture, &tileRect, &destRect)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
	}
}

// Fonction d'affichage de la map

int drawMap(SDL_Renderer *renderer, Map *map, char *ImagePath, float x_cam, SDL_Texture *tileTexture) {
	for (int i = 0 ; i < HEIGHT ; i++) {
		for (int j = 0 ; j < WIDTH ; j++) {
			if (j*PIX_RECT - x_cam > -PIX_RECT && j*PIX_RECT - x_cam < WINWIDTH) {
				if (map->matrix[i][j] == '7') {
					display_tile(renderer,0,2,j,i, x_cam, tileTexture); // Coin superieur gauche
				}
				if (map->matrix[i][j] == '4') {
					display_tile(renderer,0,3,j,i, x_cam, tileTexture); // Mur face gauche
				}
				if (map->matrix[i][j] == '1') {
					display_tile(renderer,0,4,j,i, x_cam, tileTexture); // Coin inferieur gauche
				}
				if (map->matrix[i][j] == '9') {
					display_tile(renderer,4,2,j,i, x_cam, tileTexture); // Coin superieur droit
				}
				if (map->matrix[i][j] == '6') {
					display_tile(renderer,4,3,j,i, x_cam, tileTexture); // Mur face droit 
				}
				if (map->matrix[i][j] == '3') {
					display_tile(renderer,4,4,j,i, x_cam, tileTexture); // Coin inferieur droit
				}
				if (map->matrix[i][j] == '8') {
					if (j%3==0) {
						display_tile(renderer,1,2,j,i, x_cam, tileTexture);
					} else if (j%3==1) {
						display_tile(renderer,2,2,j,i, x_cam, tileTexture);
					} else {
						display_tile(renderer,3,2,j,i, x_cam, tileTexture); // Sol
					}
				}
				if (map->matrix[i][j] == '5') {
					display_tile(renderer,1,3,j,i, x_cam, tileTexture); // Vide
				}
				if (map->matrix[i][j] == '2') {
					if (j%3==0) {
						display_tile(renderer,1,4,j,i, x_cam, tileTexture);
					} else if (j%3==1) {
						display_tile(renderer,2,4,j,i, x_cam, tileTexture);
					} else {
						display_tile(renderer,3,4,j,i, x_cam, tileTexture);// Plafond
					}
				}
				if (map->matrix[i][j] == 'u') {
					display_tile(renderer,0,1,j,i, x_cam, tileTexture); // Angle superieur droit
				}
				if (map->matrix[i][j] == 'i') {
					display_tile(renderer,1,1,j,i, x_cam, tileTexture); // Angle inferieur droit
				}
				if (map->matrix[i][j] == 'o') {
					display_tile(renderer,2,1,j,i, x_cam, tileTexture); // Angle inferieur gauche
				}
				if (map->matrix[i][j] == 'p') {
					display_tile(renderer,3,1,j,i, x_cam, tileTexture); // Angle superieur gauche
				}
				if (map->matrix[i][j] == 'a') {
					display_tile(renderer,0,0,j,i, x_cam, tileTexture); // Plateforme extremite droit
				}
				if (map->matrix[i][j] == 'z') {
					if (j%3==0) {
						display_tile(renderer,1,0,j,i, x_cam, tileTexture);
					} else if (j%3==1) {
						display_tile(renderer,2,0,j,i, x_cam, tileTexture);
					} else {
						display_tile(renderer,3,0,j,i, x_cam, tileTexture);// Plafond
					} // Plateforme corps
				}
				if (map->matrix[i][j] == 'e') {
					display_tile(renderer,4,0,j,i, x_cam, tileTexture); // Plateforme extremite gauche
				}
			}
		}
	}
	return 0 ;
}
