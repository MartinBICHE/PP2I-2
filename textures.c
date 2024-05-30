#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>


SDL_Texture* textureEnemy1 = NULL;
SDL_Texture* textureEnemy2 = NULL;
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
SDL_Texture* textureHealthBoss0 = NULL;
SDL_Texture* textureHealthBoss1 = NULL;
SDL_Texture* textureHealthBoss2 = NULL;
SDL_Texture* textureHealthBoss3 = NULL;
SDL_Texture* textureHealthBoss4 = NULL;
SDL_Texture* textureHealthBoss5 = NULL;
SDL_Texture* textureHealthBoss6 = NULL;
SDL_Texture* textureHealthBoss7 = NULL;
SDL_Texture* textureHealthBoss8 = NULL;
SDL_Texture* textureHealthBoss9 = NULL;
SDL_Texture* textureFleche = NULL;
SDL_Texture* textureBat = NULL;
SDL_Texture* textureBatAttack = NULL;
SDL_Texture* textureEnemy3Attack = NULL;
SDL_Texture* textureProjectile = NULL;
SDL_Texture* textureAttack = NULL;
SDL_Texture* texturePortail = NULL;

SDL_Texture *healthTextures[10];
SDL_Texture *healthTexturesBoss[10];





int loadTextures(SDL_Renderer *renderer){


    SDL_Surface *surfaceEnemy1 = IMG_Load("asset/spritesheet/ennemy1.png");
    SDL_Surface *surfaceEnemy2 = IMG_Load("asset/spritesheet/ennemy2.png");
    SDL_Surface *surfaceEnemy3 = IMG_Load("asset/spritesheet/ennemy3.png");
    SDL_Surface *surfacePapirus = IMG_Load("asset/spritesheet/papirus.png");
    SDL_Surface *surfaceScroll = IMG_Load("asset/spritesheet/scroll.png");
    SDL_Surface *surfacePendule = IMG_Load("asset/spritesheet/pendule.png");
    SDL_Surface *surfaceHealth0 = IMG_Load("asset/spritesheet/health0.png");
    SDL_Surface *surfaceHealth1 = IMG_Load("asset/spritesheet/health1.png");
    SDL_Surface *surfaceHealth2 = IMG_Load("asset/spritesheet/health2.png");
    SDL_Surface *surfaceHealth3 = IMG_Load("asset/spritesheet/health3.png");
    SDL_Surface *surfaceHealth4 = IMG_Load("asset/spritesheet/health4.png");
    SDL_Surface *surfaceHealth5 = IMG_Load("asset/spritesheet/health5.png");
    SDL_Surface *surfaceHealth6 = IMG_Load("asset/spritesheet/health6.png");
    SDL_Surface *surfaceHealth7 = IMG_Load("asset/spritesheet/health7.png");
    SDL_Surface *surfaceHealth8 = IMG_Load("asset/spritesheet/health8.png");
    SDL_Surface *surfaceHealth9 = IMG_Load("asset/spritesheet/health9.png");
    SDL_Surface *surfaceHealthBoss0 = IMG_Load("asset/spritesheet/heathBoss0.png");
    SDL_Surface *surfaceHealthBoss1 = IMG_Load("asset/spritesheet/heathBoss1.png");
    SDL_Surface *surfaceHealthBoss2 = IMG_Load("asset/spritesheet/heathBoss2.png");
    SDL_Surface *surfaceHealthBoss3 = IMG_Load("asset/spritesheet/heathBoss3.png");
    SDL_Surface *surfaceHealthBoss4 = IMG_Load("asset/spritesheet/heathBoss4.png");
    SDL_Surface *surfaceHealthBoss5 = IMG_Load("asset/spritesheet/heathBoss5.png");
    SDL_Surface *surfaceHealthBoss6 = IMG_Load("asset/spritesheet/heathBoss6.png");
    SDL_Surface *surfaceHealthBoss7 = IMG_Load("asset/spritesheet/heathBoss7.png");
    SDL_Surface *surfaceHealthBoss8 = IMG_Load("asset/spritesheet/heathBoss8.png");
    SDL_Surface *surfaceHealthBoss9 = IMG_Load("asset/spritesheet/heathBoss9.png");
    SDL_Surface *surfaceFleche = IMG_Load("asset/spritesheet/fleche.png");
    // SDL_Surface *surfaceBat = IMG_Load("asset/spritesheet/Bat.png");
    // SDL_Surface *surfaceBatAttack = IMG_Load("asset/spritesheet/batattack.png");
    // SDL_Surface *surfaceEnemy3Attack = IMG_Load("asset/spritesheet/ennemy3att.png");
    SDL_Surface *surfaceProjectile = IMG_Load("asset/spritesheet/projectiles.png");
    SDL_Surface *surfaceAttack = IMG_Load("asset/spritesheet/attack.png");
    SDL_Surface *surfacePortail = IMG_Load("asset/spritesheet/portail.png");
    SDL_Surface *surfaceBat = IMG_Load("asset/spritesheet/bat.png");
    SDL_Surface *surfaceBatAttack = IMG_Load("asset/spritesheet/batAttack.png");
    SDL_Surface *surfaceEnemy3Attack = IMG_Load("asset/spritesheet/ennemy3Attack.png");
    if (!surfaceEnemy1 || !surfacePapirus || !surfaceScroll || !surfaceEnemy3 || !surfacePendule ){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init surfaceEnemy1 or surfacePendule or.... etc : %s", SDL_GetError());
		exit(-1);
	}

    textureEnemy1 = SDL_CreateTextureFromSurface(renderer, surfaceEnemy1);
    textureEnemy2 = SDL_CreateTextureFromSurface(renderer, surfaceEnemy2);
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
    textureHealthBoss0 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss0);
    textureHealthBoss1 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss1);
    textureHealthBoss2 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss2);
    textureHealthBoss3 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss3);
    textureHealthBoss4 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss4);
    textureHealthBoss5 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss5);
    textureHealthBoss6 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss6);
    textureHealthBoss7 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss7);
    textureHealthBoss8 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss8);
    textureHealthBoss9 = SDL_CreateTextureFromSurface(renderer, surfaceHealthBoss9);
    textureFleche = SDL_CreateTextureFromSurface(renderer, surfaceFleche);
    textureBat = SDL_CreateTextureFromSurface(renderer, surfaceBat);
    textureBatAttack = SDL_CreateTextureFromSurface(renderer, surfaceBatAttack);
    textureEnemy3Attack = SDL_CreateTextureFromSurface(renderer, surfaceEnemy3Attack);
    textureProjectile = SDL_CreateTextureFromSurface(renderer, surfaceProjectile);
    textureAttack = SDL_CreateTextureFromSurface(renderer, surfaceAttack);
    texturePortail = SDL_CreateTextureFromSurface(renderer, surfacePortail);

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

    healthTexturesBoss[0] = textureHealthBoss0;
    healthTexturesBoss[1] = textureHealthBoss1;
    healthTexturesBoss[2] = textureHealthBoss2;
    healthTexturesBoss[3] = textureHealthBoss3;
    healthTexturesBoss[4] = textureHealthBoss4;
    healthTexturesBoss[5] = textureHealthBoss5;
    healthTexturesBoss[6] = textureHealthBoss6;
    healthTexturesBoss[7] = textureHealthBoss7;
    healthTexturesBoss[8] = textureHealthBoss8;
    healthTexturesBoss[9] = textureHealthBoss9;

    SDL_FreeSurface(surfaceEnemy1);
    SDL_FreeSurface(surfaceEnemy2);
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

    SDL_FreeSurface(surfaceHealthBoss0);
    SDL_FreeSurface(surfaceHealthBoss1);
    SDL_FreeSurface(surfaceHealthBoss2);
    SDL_FreeSurface(surfaceHealthBoss3);
    SDL_FreeSurface(surfaceHealthBoss4);
    SDL_FreeSurface(surfaceHealthBoss5);
    SDL_FreeSurface(surfaceHealthBoss6);
    SDL_FreeSurface(surfaceHealthBoss7);
    SDL_FreeSurface(surfaceHealthBoss8);
    SDL_FreeSurface(surfaceHealthBoss9);

    SDL_FreeSurface(surfaceFleche);
    SDL_FreeSurface(surfaceBat);
    SDL_FreeSurface(surfaceBatAttack);
    SDL_FreeSurface(surfaceEnemy3Attack);
    SDL_FreeSurface(surfaceProjectile);
    SDL_FreeSurface(surfaceAttack);
    SDL_FreeSurface(surfacePortail);

    return 0;
}

int freeTextures(void){

    SDL_DestroyTexture(textureEnemy1);
    SDL_DestroyTexture(textureEnemy2);
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

    SDL_DestroyTexture(textureHealthBoss0);
    SDL_DestroyTexture(textureHealthBoss1);
    SDL_DestroyTexture(textureHealthBoss2);
    SDL_DestroyTexture(textureHealthBoss3);
    SDL_DestroyTexture(textureHealthBoss4);
    SDL_DestroyTexture(textureHealthBoss5);
    SDL_DestroyTexture(textureHealthBoss6);
    SDL_DestroyTexture(textureHealthBoss7);
    SDL_DestroyTexture(textureHealthBoss8);
    SDL_DestroyTexture(textureHealthBoss9);

    SDL_DestroyTexture(textureBat);
    SDL_DestroyTexture(textureFleche);
    SDL_DestroyTexture(textureBatAttack);
    SDL_DestroyTexture(textureEnemy3Attack);
    SDL_DestroyTexture(textureProjectile);
    SDL_DestroyTexture(texturePortail);

    textureEnemy1 = NULL;
    textureEnemy2 = NULL;
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

    textureHealthBoss0 = NULL;
    textureHealthBoss1 = NULL;
    textureHealthBoss2 = NULL;
    textureHealthBoss3 = NULL;
    textureHealthBoss4 = NULL;
    textureHealthBoss5 = NULL;
    textureHealthBoss6 = NULL;
    textureHealthBoss7 = NULL;
    textureHealthBoss8 = NULL;
    textureHealthBoss9 = NULL;

    textureFleche = NULL;
    textureBat = NULL;
    textureBatAttack = NULL;
    textureEnemy3Attack = NULL;

    return 0;
}
