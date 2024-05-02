#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "pendule.h"
#include <math.h>
#include "textures.h"

/* pour l'utiliser: */

/* pendule_mouvement(renderer, &penduleData(à initialiser avec la fonction init)) */
/* PenduleData penduleData; */
/* initPendule(&penduleData, texturePendule, xPos(à définir), yPost(à définir")) */

void pendule_mouvement(SDL_Renderer *renderer, PenduleData *penduleData){
    float angle = penduleData->amplitude * cos(2 * PI * penduleData->time / penduleData->period + PI);
    penduleData->x = 30*penduleData->amplitude * sin(angle) + penduleData->xInit;
    penduleData->y = -30*penduleData->amplitude * cos(angle) + penduleData->yInit;
    penduleData->time += 0.05;
    SDL_Rect dest_rect = {penduleData->x, penduleData->y, penduleData->scale*penduleData->textureWidth, penduleData->scale*penduleData->textureHeight};
    SDL_RenderCopy(renderer, texturePendule, NULL, &dest_rect);
}


void initPendule(PenduleData *penduleData, SDL_Texture *texture, int xInit, int yInit){
    penduleData->amplitude = -1 * PI/2.0; 
    penduleData->period = 4;
    penduleData->time = 0;
    penduleData->scale = 3;
    Uint32 format;
    int access, width, height;
    SDL_QueryTexture(texture, &format, &access, &width, &height);
    penduleData->textureHeight = width ;
    penduleData->textureWidth = height;
    penduleData->xInit = xInit;
    penduleData->yInit = yInit;
}
