#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include "pendule.h"
#include <math.h>

/* pour l'utiliser: */

/* pendule_mouvement(renderer, texturePendule, xPos(à mettre une valeur), yPos(à mettre une valeur)), &penduleData(à initialiser avec la fonction init)) */
/* PenduleData penduleData; */
/* initPendule(&penduleData, texturePendule) */

void pendule_mouvement(SDL_Renderer *renderer, SDL_Texture *texture, int xInit, int yInit, PenduleData *penduleData){
    float angle = penduleData->amplitude * cos(2 * PI * penduleData->time / penduleData->period + PI);
    penduleData->x = 30*penduleData->amplitude * sin(angle) + xInit;
    penduleData->y = -30*penduleData->amplitude * cos(angle) + yInit;
    penduleData->time += 0.05;
    SDL_Rect dest_rect = {penduleData->x, penduleData->y, penduleData->scale*penduleData->textureWidth, penduleData->scale*penduleData->textureHeight};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
}


void initPendule(PenduleData *penduleData, SDL_Texture *texture){
    penduleData->amplitude = -1 * PI/2.0; 
    penduleData->period = 4;
    penduleData->time = 0;
    penduleData->scale = 3;
    Uint32 format;
    int access, width, height;
    SDL_QueryTexture(texture, &format, &access, &width, &height);
    penduleData->textureHeight = width ;
    penduleData->textureWidth = height;
}
