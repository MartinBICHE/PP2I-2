#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

void SDL_ExitWithError(const char *message);

int main(int argc, const char * argv[]) {
    SDL_version nb;
    SDL_VERSION(&nb);

    printf("Bienvenue sur la SDL %d,%d,%d!\n", nb.major, nb.minor,nb.patch);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if(SDL_Init( SDL_INIT_VIDEO ) != 0){
        SDL_Log("Erreur : Initialisation SDL > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Scarf",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,0);


    if (window == NULL) {
        SDL_Log("Erreur : Initialisation SDL > %s\n", SDL_GetError());
        exit(EXIT_FAILURE); 
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == NULL) {
        SDL_ExitWithError("Creation de rendu échouée");

    }

    SDL_RenderPresent(renderer);

    SDL_Delay(10000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void SDL_ExitWithError(const char *message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit;
    exit(EXIT_FAILURE);
}


