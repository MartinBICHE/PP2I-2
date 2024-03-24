#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define ANIMATION_FRAME_RATE 12

int spriteMovement(SDL_Renderer *renderer, SDL_Event event, int *distance, const char *ImagePath) {
    static int offset = 0;
    static int line = 0;
    int left = 0;
    int idle = 1;
    static Uint32 lastFrameTime = 0;

    // Calculate time elapsed since the last frame
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - lastFrameTime;

    

    static SDL_Texture *spriteTexture = NULL;
    if (!spriteTexture) {
    SDL_Surface *spriteSurface = IMG_Load(ImagePath);
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
                idle=0;
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        left = 0;
                        line = 1;
                        offset++;
                        offset %= 12;
                        *distance += 15; 
                        break;
                    case SDLK_LEFT:
                        left = 1;
                        line = 1; 
                        offset++;
                        offset %= 12;
                        *distance -= 15;  
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                idle = 1;
                offset = 0;
                break;
            default:
                break;
        }
    }

    if (elapsedTime >= 1000 / ANIMATION_FRAME_RATE) {
        lastFrameTime = currentTime;
        if (idle == 1) {
            line = 0;
            offset++;
            offset %= 12;
        }
    }

    SDL_RendererFlip flip = (left == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;


    SDL_Rect spriteRect = {.x = offset * spriteWidth, .y = line * spriteHeight, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = { .x = *distance, .y = 0, .w = spriteWidth, .h = spriteHeight};
    if (SDL_RenderCopyEx(renderer, spriteTexture, &spriteRect, &destRect, 0, NULL, flip)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in render copy: %s", SDL_GetError());
        exit(-1);
    }
    SDL_RenderPresent(renderer);
    return 0;
}
