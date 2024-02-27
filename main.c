#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, const char * argv[]) {
    SDL_version nb;
    SDL_VERSION(&nb);

    printf("Bienvenue sur la SDL %d,%d,%d!\n", nb.major, nb.minor,nb.patch);

    SDL_Init( SDL_INIT_VIDEO );
    return 0;
}