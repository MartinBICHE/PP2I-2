#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "const.h"
#include "fight.h"

int fightMovement(SDL_Renderer *renderer, SDL_Event event, Perso *player) {
    static int offset = 0;
    static int line = 0;

    static SDL_Texture *spriteTexture = NULL;
    if (!spriteTexture) {
        SDL_Surface *spriteSurface = IMG_Load("./asset/spritesheet/ss_mc.png");
        if (!spriteSurface) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface init: %s", IMG_GetError());
            exit(-1);
        }
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        if (!spriteTexture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
            exit(-1);
        }
        SDL_FreeSurface(spriteSurface);
    }

    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture,NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteHeight = spriteFullHeight / 4; 
    int spriteWidth = spriteFullWidth / 12;

    if (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        if ( player->x <= WINWIDTH-TIERWIDTH) {
                            player->x += TIERWIDTH;
                        }
                        break;
                    case SDLK_LEFT:
                        if (player->x > TIERWIDTH) {
                            player->x -= TIERWIDTH;
                        }
                        break;
                    case SDLK_DOWN:
                        if ( player->y < WINHEIGHT-QUARTERHEIGHT) {
                            player->y += 2*QUARTERHEIGHT;
                        }
                        break;
                    case SDLK_UP:
                        if (player->y > QUARTERHEIGHT/2) {
                            player->y -= 2*QUARTERHEIGHT;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    SDL_Rect spriteRect = {.x = offset * spriteWidth, .y = line * spriteHeight, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = { .x = player->x, .y = player->y, .w = spriteWidth, .h = spriteHeight};

    if (SDL_RenderCopyEx(renderer, spriteTexture, &spriteRect, &destRect, 0, NULL, SDL_FLIP_NONE)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in render copy: %s", SDL_GetError());
        exit(-1);
    }
    SDL_RenderPresent(renderer);

    return 0;
}


int showRectangle(SDL_Renderer* renderer, int x, int y, int w, int h) {
    int screenWidth = 0;
    int screenHeight = 0;
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;


    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_Rect destRect = { .x = x, .y = y, .w = w, .h = h};
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &destRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    return 0;
}
