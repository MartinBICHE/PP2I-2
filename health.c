#include "health.h"
#include "const.h"
#include "textures.h"
#include <SDL2/SDL_render.h>

void renderStatusHealth(SDL_Renderer *renderer, Perso *perso){
    SDL_Rect dst_rect = {-10, -80, 128*2, 128*2};
    SDL_RenderCopy(renderer, healthTextures[perso->health], NULL, &dst_rect);

}