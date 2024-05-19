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
SDL_Texture* textureHealth0 = NULL;
SDL_Texture* textureHealth1 = NULL;
SDL_Texture* textureHealth2 = NULL;
SDL_Texture* textureHealth3 = NULL;
SDL_Texture* textureHealth4 = NULL;
SDL_Texture* textureHealth5 = NULL;
SDL_Texture* textureHealth6 = NULL;
SDL_Texture* textureHealth7 = NULL;
SDL_Texture* textureHealth8 = NULL;
SDL_Texture* textureHealth9 = NULL;
SDL_Texture* textureFleche = NULL;
SDL_Texture* textureBat = NULL;

SDL_Texture *healthTextures[10];





int loadTextures(SDL_Renderer *renderer){


    SDL_Surface *surfaceEnemy1 = IMG_Load("asset/spritesheet/ennemy1.png");
    SDL_Surface *surfaceEnemy3 = IMG_Load("asset/spritesheet/ennemy3.png");
    SDL_Surface *surfacePapirus = IMG_Load("asset/spritesheet/papirus.png");
    SDL_Surface *surfaceScroll = IMG_Load("asset/spritesheet/scroll.png");
    SDL_Surface *surfacePendule = IMG_Load("asset/spritesheet/pendule.png");
    SDL_Surface *surfaceHealth0 = IMG_Load("asset/spritesheet/health0.png");
    SDL_Surface *surfaceHealth1 = IMG_Load("asset/spritesheet/health1.png");
    SDL_Surface *surfaceHealth2 = IMG_Load("asset/spritesheet/health2.png");
    SDL_Surface *surfaceHealth3 = IMG_Load("asset/spritesheet/health3.png");
    SDL_Surface *surfaceHealth4 = IMG_Load("asset/spritesheet/health4.png");
    SDL_Surface *surfaceHealth5 = IMG_Load("asset/spritesheet/health4.png");
    SDL_Surface *surfaceHealth6 = IMG_Load("asset/spritesheet/health6.png");
    SDL_Surface *surfaceHealth7 = IMG_Load("asset/spritesheet/health7.png");
    SDL_Surface *surfaceHealth8 = IMG_Load("asset/spritesheet/health8.png");
    SDL_Surface *surfaceHealth9 = IMG_Load("asset/spritesheet/health9.png");
    SDL_Surface *surfaceFleche = IMG_Load("asset/spritesheet/fleche.png");
    SDL_Surface *surfaceBat = IMG_Load("asset/spritesheet/bat.png");
    if (!surfaceEnemy1 || !surfacePapirus || !surfaceScroll || !surfaceEnemy3 || !surfacePendule ){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init surfaceEnemy1 or surfacePendule or.... etc : %s", SDL_GetError());
		exit(-1);
	}

    textureEnemy1 = SDL_CreateTextureFromSurface(renderer, surfaceEnemy1);
    textureEnemy3 = SDL_CreateTextureFromSurface(renderer, surfaceEnemy3);
    texturePapirus = SDL_CreateTextureFromSurface(renderer, surfacePapirus);
    textureScroll = SDL_CreateTextureFromSurface(renderer, surfaceScroll);
    texturePendule = SDL_CreateTextureFromSurface(renderer, surfacePendule);
    texturePendule = SDL_CreateTextureFromSurface(renderer, surfacePendule);
    textureHealth0 = SDL_CreateTextureFromSurface(renderer, surfaceHealth0);
    textureHealth1 = SDL_CreateTextureFromSurface(renderer, surfaceHealth1);
    textureHealth2 = SDL_CreateTextureFromSurface(renderer, surfaceHealth2);
    textureHealth3 = SDL_CreateTextureFromSurface(renderer, surfaceHealth3);
    textureHealth4 = SDL_CreateTextureFromSurface(renderer, surfaceHealth4);
    textureHealth5 = SDL_CreateTextureFromSurface(renderer, surfaceHealth5);
    textureHealth6 = SDL_CreateTextureFromSurface(renderer, surfaceHealth6);
    textureHealth7 = SDL_CreateTextureFromSurface(renderer, surfaceHealth7);
    textureHealth8 = SDL_CreateTextureFromSurface(renderer, surfaceHealth8);
    textureHealth9 = SDL_CreateTextureFromSurface(renderer, surfaceHealth9);
    textureFleche = SDL_CreateTextureFromSurface(renderer, surfaceFleche);
    textureBat = SDL_CreateTextureFromSurface(renderer, surfaceBat);

    if (!textureEnemy1 || !texturePapirus || !textureScroll || !textureEnemy3 || !texturePendule){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init textureEnemy1 or surfacePendule or.... etc : %s", SDL_GetError());
		exit(-1);
	}

    healthTextures[0] = textureHealth0;
    healthTextures[1] = textureHealth1;
    healthTextures[2] = textureHealth2;
    healthTextures[3] = textureHealth3;
    healthTextures[4] = textureHealth4;
    healthTextures[5] = textureHealth5;
    healthTextures[6] = textureHealth6;
    healthTextures[7] = textureHealth7;
    healthTextures[8] = textureHealth8;
    healthTextures[9] = textureHealth9;


    SDL_FreeSurface(surfaceEnemy1);
    SDL_FreeSurface(surfaceEnemy3);
    SDL_FreeSurface(surfacePapirus);
    SDL_FreeSurface(surfaceScroll);
    SDL_FreeSurface(surfacePendule);

    SDL_FreeSurface(surfaceHealth0);
    SDL_FreeSurface(surfaceHealth1);
    SDL_FreeSurface(surfaceHealth2);
    SDL_FreeSurface(surfaceHealth3);
    SDL_FreeSurface(surfaceHealth4);
    SDL_FreeSurface(surfaceHealth5);
    SDL_FreeSurface(surfaceHealth6);
    SDL_FreeSurface(surfaceHealth7);
    SDL_FreeSurface(surfaceHealth8);
    SDL_FreeSurface(surfaceHealth9);
    SDL_FreeSurface(surfaceFleche);
    SDL_FreeSurface(surfaceBat);

    return 0;
}

int freeTextures(void){

    SDL_DestroyTexture(textureEnemy1);
    SDL_DestroyTexture(textureEnemy3);
    SDL_DestroyTexture(textureScroll);
    SDL_DestroyTexture(texturePapirus);
    SDL_DestroyTexture(texturePendule);

    SDL_DestroyTexture(texturePendule);
    SDL_DestroyTexture(textureHealth0);
    SDL_DestroyTexture(textureHealth1);
    SDL_DestroyTexture(textureHealth2);
    SDL_DestroyTexture(textureHealth3);
    SDL_DestroyTexture(textureHealth4);
    SDL_DestroyTexture(textureHealth5);
    SDL_DestroyTexture(textureHealth6);
    SDL_DestroyTexture(textureHealth7);
    SDL_DestroyTexture(textureHealth8);
    SDL_DestroyTexture(textureHealth9);
    SDL_DestroyTexture(textureBat);
    SDL_DestroyTexture(textureFleche);

    textureEnemy1 = NULL;
    textureEnemy3 = NULL;
    texturePendule = NULL;
    texturePapirus = NULL;
    textureScroll = NULL;

    textureHealth0 = NULL;
    textureHealth1 = NULL;
    textureHealth2 = NULL;
    textureHealth3 = NULL;
    textureHealth4 = NULL;
    textureHealth5 = NULL;
    textureHealth6 = NULL;
    textureHealth7 = NULL;
    textureHealth8 = NULL;
    textureHealth9 = NULL;
    textureFleche = NULL;
    textureBat = NULL;

    return 0;
}
