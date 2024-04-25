#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT = 50;
const float CHARACTER_SPEED = 0.5;
const float JUMP_SPEED = 10;
const float GRAVITY = 0.5;
const int JUMP_DELAY = 500;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Test déplacement personnage", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    // Charger l'image de l'arrière-plan
    SDL_Surface *backgroundSurface = IMG_Load("91655.jpg");
    if (!backgroundSurface) {
        printf("Impossible de charger l'image de  l'arrière-plan : %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Charger l'image du personnage
    SDL_Surface *characterSurface = IMG_Load("idle_mc_hd.gif");
    if (!characterSurface) {
        printf("Impossible de charger l'image du personnage : %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface);

    // Position initiale du personnage
    float characterX = 0;
    float characterY = WINDOW_HEIGHT - CHARACTER_HEIGHT;
    int direction = 1; // Direction initiale du personnage
    bool jumping = false; // Indique si le personnage est en train de sauter
    float jumpVelocity = 0; // Vitesse de saut
    Uint32 lastJumpTime = 0; // Moment du dernier saut;
    bool quit = false; // Indique si le programme doit se terminer
    SDL_Texture *texture = NULL;

    // Boucle principale du jeu
    while (!quit) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true; // Quitter le programme si l'utilisateur ferme la fenêtre
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE && !jumping && (SDL_GetTicks() - lastJumpTime) > JUMP_DELAY) {
                    // Démarrer un saut si la barre d'espace est enfoncée et que le personnage n'est pas déjà en train de sauter
                    jumping = true;
                    jumpVelocity = JUMP_SPEED;
                    lastJumpTime = SDL_GetTicks();
                }
            }
        }

        // Mise à jour de la position du personnage
        if (jumping) {
            characterY -= jumpVelocity;
            jumpVelocity -= GRAVITY;
            if (jumpVelocity <= 0) {
                jumping = false; // Fin du saut lorsque la vitesse de saut atteint 0
            }
        } else {
            if (characterY < WINDOW_HEIGHT - CHARACTER_HEIGHT) {
                characterY += GRAVITY; // Appliquer la gravité lorsque le personnage n'est pas en train de sauter
            }
        }
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] && characterX > 0) {
            characterX -= CHARACTER_SPEED; // Déplacer le personnage vers la gauche
            direction = -1; // Mettre à jour la direction du personnage
        }
        if (state[SDL_SCANCODE_RIGHT] && characterX < WINDOW_WIDTH - CHARACTER_WIDTH) {
            characterX += CHARACTER_SPEED; // Déplacer le personnage vers la droite
            direction = 1; // Mettre à jour la direction du personnage
        }

        // Affichage de l'arrière-plan
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Affichage du personnage
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect characterRect = {(int)characterX, (int)characterY, CHARACTER_WIDTH, CHARACTER_HEIGHT};
        SDL_RenderCopyEx(renderer, characterTexture, NULL, &characterRect, 0, NULL, (direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        SDL_RenderPresent(renderer); // Rafraîchir l'affichage
    }

    // Nettoyage et sortie du programme
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}