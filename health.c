#include "health.h"
#include "const.h"
#include "textures.h"
#include <SDL2/SDL_render.h>
#include "checkpoints.h"

void renderStatusHealth(SDL_Renderer *renderer, Perso *perso){
    SDL_Rect dst_rect = {-10, -80, 128*2, 128*2};
    if (perso->health >= 0 && perso->health <= 9){
        SDL_RenderCopy(renderer, healthTextures[perso->health], NULL, &dst_rect);
    }
}

void handleDeath(Perso *perso, const char *filename) {
    if (perso->health <= 0) {
        puts("Player died! Saving position before respawn...");
        perso->health = 9;
        savePosition(filename, perso);
        SDL_Quit(); 
    }
}
