#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "const.h"
#include "map.h"

#define ANIMATION_FRAME_RATE 12
#define JUMP_FORCE 100

// Déclarer une variable globale pour stocker la dernière direction du déplacement
int lastDirection = 1; //  1 pour droite, 0 pour gauche

int spriteMovement(SDL_Renderer *renderer, SDL_Event event, int *distance, int *y_position, const char *ImagePath, Map *map) {
    static int offset = 0;
    static int line = 0;
    int left = 0;
    int idle = 1;
    int jumping = 0; // Variable pour gérer l'état du saut
    int jumpHeight = 0; // Variable pour suivre la hauteur du saut
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
                        lastDirection = 1; // Mettre à jour la dernière direction
                        line = 1;
                        // Vérifier la collision avec le mur avant de permettre le déplacement
                        if (!detect_wall_collision(map, *distance + 15, *y_position)) {
                            offset++;
                            offset %= 12;
                            *distance += 15;
                        } 
                        break;
                    case SDLK_LEFT:
                        left = 1;
                        lastDirection = 0; // Mettre à jour la dernière direction
                        line = 1;
                        // Vérifier la collision avec le mur avant de permettre le déplacement
                        if (!detect_wall_collision(map, *distance - 15, *y_position)) {
                            offset++;
                            offset %= 12;
                            *distance -= 15; 
                        } 
                        break;
                    case SDLK_SPACE: // Déclencher un saut lorsque la touche d'espace est enfoncée
                        if (!jumping) {
                            jumping = 1;
                            jumpHeight = JUMP_FORCE;
                        }
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

    // Logique pour déterminer la direction du sprite lorsque le personnage est immobile
    if (idle == 1) {
        left = lastDirection == 0; // Utiliser la dernière direction;
    }

    // Logique de saut
    if (jumping) {
        jumpHeight -= GRAVITY;
        *y_position -= jumpHeight; // Ajuster la position verticale pendant le saut
        if (jumpHeight <= 0) {
            jumping = 0;
        }
    } else { 
        // Logique de gravité
        // Mise à jour de la position horizontale en fonction des touches de déplacement gauche et droite
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] && !detect_wall_collision(map, *distance - 15, *y_position)) {
            *distance -= 15; // Déplacer le personnage vers la gauche
        }
        if (state[SDL_SCANCODE_RIGHT] && !detect_wall_collision(map, *distance + 15, *y_position)) {
            *distance += 15; // Déplacer le personnage vers la droite
        }

        int windowWidth, windowHeight;
        SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
        if (*y_position + spriteHeight >= windowHeight) {
            // Si le personnage touche le sol, arrêter la descente
            *y_position = windowHeight - spriteHeight;
        } else {
            *y_position += GRAVITY;
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
    SDL_Rect destRect = { .x = *distance, .y = *y_position, .w = spriteWidth, .h = spriteHeight}; // Utiliser y_position pour la position verticale
    if (SDL_RenderCopyEx(renderer, spriteTexture, &spriteRect, &destRect, 0, NULL, flip)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in render copy: %s", SDL_GetError());
        exit(-1);
    }
    SDL_RenderPresent(renderer);
    return 0;
}
