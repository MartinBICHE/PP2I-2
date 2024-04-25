#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "const.h"
#include "main.h"

enum EnemyState {
    MOVING_UP,
    PAUSE_TOP,
    MOVING_DOWN,
    PAUSE_BOTTOM,
    ANIMATION_START
};

/* à utiliser avec: */ 
/*         enemy1_movement(renderer, texture, &src_rect, &dest_rect, &b); */
/*     SDL_Rect dest_rect = {460, 230, 64, 0}; */
/*     SDL_Rect src_rect = {0, 0, 64, 0}; nécessairement le src_rect défini de cette manière */

void enemy1_movement(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src_rect, SDL_Rect *dst_rect, int *i) {
    int speed = 5;
    int interval = 2000;
    static enum EnemyState state = MOVING_UP;
    static Uint32 pauseStart = 0;
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 500) % 10;

    switch (state) {
        case MOVING_UP:
            if (dst_rect->h <= 192) {
                dst_rect->h += speed;
                dst_rect->y -= speed;
                src_rect->h += speed;
            } else {
                state = ANIMATION_START; 
            }
            break;
        case ANIMATION_START:
            src_rect->x += 64;
            *i = (*i + 1); 
            src_rect->x = sprite * 64;

            if (sprite == 9) { 
                state = PAUSE_TOP;
                pauseStart = SDL_GetTicks();
            }
            break;
        case PAUSE_TOP:
            if (SDL_GetTicks() - pauseStart >= interval) { 
                state = MOVING_DOWN;
            }
            break;
        case MOVING_DOWN:
            if (dst_rect->h >= 0) {
                dst_rect->h -= speed;
                dst_rect->y += speed;
                src_rect->h -= speed;
            } else {
                state = PAUSE_BOTTOM;
                pauseStart = SDL_GetTicks();
            }
            break;
        case PAUSE_BOTTOM:
            if (SDL_GetTicks() - pauseStart >= interval) { 
                state = MOVING_UP;
            }
            break;
    }

    SDL_RenderCopy(renderer, texture, src_rect, dst_rect);
}


