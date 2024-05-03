#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "const.h"

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

    const int INPUT_DELAY_MS = 3; 
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    static clock_t lastInputTime = 0;
    clock_t currentTime = clock();

    if ((double)(currentTime - lastInputTime) / CLOCKS_PER_SEC * 1000 >= INPUT_DELAY_MS) {
        lastInputTime = currentTime;
        if (keyboardState[SDL_SCANCODE_UP]) {
            if (player->y >  QUARTERHEIGHT) {
                player->y -= 2 * QUARTERHEIGHT;
            }
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            if ( player->y < WINHEIGHT - 2 * QUARTERHEIGHT - 1.5*SPRITESIZE) {
                player->y += 2 * QUARTERHEIGHT;
            }
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            if (player->x > TIERWIDTH) {
                player->x -= TIERWIDTH;
            }
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            if ( player->x <= WINWIDTH-TIERWIDTH) {
                player->x += TIERWIDTH;
            }
        }
    }


    SDL_Rect spriteRect = {.x = offset * spriteWidth, .y = line * spriteHeight, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = { .x = player->x, .y = player->y, .w = 1.5*spriteWidth, .h = 1.5*spriteHeight};

    if (SDL_RenderCopyEx(renderer, spriteTexture, &spriteRect, &destRect, 0, NULL, SDL_FLIP_NONE)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in render copy: %s", SDL_GetError());
        exit(-1);
    }
    SDL_RenderPresent(renderer);

    return 0;
}


// // Function to display random-sized and randomly located rectangles
// int showRectangle(SDL_Renderer* renderer) {
//     // Seed the random number generator
//     srand(time(NULL));

//     // Generate random position and size for the rectangle
//     int x = rand() % WINWIDTH;
//     int y = rand() % WINHEIGHT;
//     int w = rand() % (WINWIDTH / 2); // Random width up to half of the window width
//     int h = rand() % (WINHEIGHT / 2); // Random height up to half of the window height

//     // Set random color
//     SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);

//     // Draw the rectangle
//     SDL_Rect rect = { .x = x, .y = y, .w = w, .h = h };
//     SDL_RenderFillRect(renderer, &rect);

//     // Update the screen
//     SDL_RenderPresent(renderer);

//     // Delay for 1 second
//     SDL_Delay(1000);

//     // Clear the renderer
//     SDL_RenderClear(renderer);

//     return 0;
// }



int showRectangle(SDL_Renderer *renderer, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);    
    return 0;
}

// Function to perform the low range attack
int lowRangeAttack(SDL_Renderer *renderer) {
    // Initialize variables
    float x = 0;
    float y = 2 * QUARTERHEIGHT;
    float w = TIERWIDTH;
    float h = 2 * QUARTERHEIGHT;
    
    Uint32 lastDrawTime = SDL_GetTicks(); // Get the current time
    
    // Main loop
    while (x <= WINWIDTH) {
        // Check if it's time to proceed to the next turn
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastDrawTime >= 1000) { // Adjust the delay duration as needed
            // Perform the action for the turn
            showRectangle(renderer, x, y, w, h);
            
            // Update the position for the next turn
            x += TIERWIDTH;
            
            // Update the last draw time
            lastDrawTime = currentTime;
        }
    }
    
    return 0;
}
