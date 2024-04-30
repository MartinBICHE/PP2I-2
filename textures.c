#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>


SDL_Texture* textureEnemy1 = NULL;
SDL_Texture* textureEnemy3 = NULL;
SDL_Texture* texturePapirus = NULL;
SDL_Texture* textureScroll = NULL;
SDL_Texture* texturePendule = NULL;


int loadTextures(SDL_Renderer *renderer){


    SDL_Surface *surfaceEnemy1 = IMG_Load("asset/spritesheet/ennemy1.png");
    SDL_Surface *surfaceEnemy3 = IMG_Load("asset/spritesheet/ennemy3.png");
    SDL_Surface *surfacePapirus = IMG_Load("asset/spritesheet/papirus.png");
    SDL_Surface *surfaceScroll = IMG_Load("asset/spritesheet/scroll.png");
    SDL_Surface *surfacePendule = IMG_Load("asset/spritesheet/pendule.png");
    if (!surfaceEnemy1 || !surfacePapirus || !surfaceScroll || !surfaceEnemy3 || !surfacePendule){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init surfaceEnemy1 or surfacePendule or.... etc : %s", SDL_GetError());
		exit(-1);
	}

    textureEnemy1 = SDL_CreateTextureFromSurface(renderer, surfaceEnemy1);
    textureEnemy3 = SDL_CreateTextureFromSurface(renderer, surfaceEnemy3);
    texturePapirus = SDL_CreateTextureFromSurface(renderer, surfacePapirus);
    textureScroll = SDL_CreateTextureFromSurface(renderer, surfaceScroll);
    texturePendule = SDL_CreateTextureFromSurface(renderer, surfacePendule);

    if (!textureEnemy1 || !texturePapirus || !textureScroll || !textureEnemy3 || !texturePendule){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init textureEnemy1 or surfacePendule or.... etc : %s", SDL_GetError());
		exit(-1);
	}

    SDL_FreeSurface(surfaceEnemy1);
    SDL_FreeSurface(surfaceEnemy3);
    SDL_FreeSurface(surfacePapirus);
    SDL_FreeSurface(surfaceScroll);
    SDL_FreeSurface(surfacePendule);


    return 0;
}

int freeTextures(){

    SDL_DestroyTexture(textureEnemy1);
    SDL_DestroyTexture(textureEnemy3);
    SDL_DestroyTexture(textureScroll);
    SDL_DestroyTexture(texturePapirus);
    SDL_DestroyTexture(texturePendule);

    textureEnemy1 = NULL;
    textureEnemy3 = NULL;
    texturePendule = NULL;
    texturePapirus = NULL;
    textureScroll = NULL;
    return 0;
}
