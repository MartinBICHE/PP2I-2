#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "display.h"
#include "enemy3.h"


    /* enemy3_movement(renderer, texture_2, &src_rect1, &dest_rect1, enemy3, 1); */
    /* SDL_Surface *surface_2 = IMG_Load("ennemy3.png"); */
    /* SDL_Texture *texture_2 = SDL_CreateTextureFromSurface(renderer, surface_2); */
    /* SDL_Rect dest_rect1 = {50, 100, 64, 64}; */
    /* SDL_Rect src_rect1 = {0, 0, 64, 64}; */
    /* Enemy3 *enemy3 = malloc(sizeof(Enemy3)); */
    /* enemy3->image_path = "bubble.png"; */
    /* enemy3->speed = 25; */
    /* enemy3->xPosition = 1*PIX_RECT; */
    /* enemy3->yPosition = 4*PIX_RECT ; */
    /* enemy3->xCollisionMax = 5*PIX_RECT; */
    /* enemy3->xCollisionMin = 1*PIX_RECT; */

void enemy3_movement(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src_rect, SDL_Rect *dst_rect, Enemy3 *enemy, float scale ){
    static int dx = 1;
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 400) % 9;

    enemy->xPosition += dx * enemy->speed * 0.1;
    if (enemy->xPosition <=  enemy->xCollisionMin || enemy->xPosition >= enemy->xCollisionMax ) {
        dx *= -1;
    }
    src_rect->x = sprite * 64;
    dst_rect->x += dx * enemy->speed * 0.1;
    if (dx == 1){
        SDL_RenderCopyEx(renderer, texture, src_rect, dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (dx == -1){
        SDL_RenderCopy(renderer, texture, src_rect, dst_rect);
    }
}


