#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "display.h"
#include "enemy3.h"



void enemy3_movement(SDL_Renderer *renderer, SDL_Texture *texture, Enemy3 *enemy, float scale ){
    static int dx = 1;
    render_sprite(renderer, texture, enemy->xPosition, enemy->yPosition, scale, enemy->image_path );
    enemy->xPosition += dx * enemy->speed * 0.1;
    if (enemy->xPosition <=  enemy->xCollisionMin || enemy->xPosition >= enemy->xCollisionMax ) {
        dx *= -1;
    }
}


