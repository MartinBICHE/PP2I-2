#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "const.h"
#include "SDL2/SDL_ttf.h"

enum ScrollState{
    MOVING_UP,
    MOVING_DOWN,
    PAUSE_BOTTOM,
    SECOND_PAUSE,
};

void scroll_movement(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *src_rect, SDL_Rect *dst_rect, TTF_Font *font, const char *text, SDL_Color color){
    int speed = 1;
    int interval = 6000;
    static enum ScrollState state = MOVING_DOWN;
    static Uint32 pauseStart = 0;

    static int currentCharIndex = 0;
    int textLength = strlen(text);
    int delay = 90;
    static Uint32 delayTimer = 0;
    const int pad = 40;
    /* dst_rectText->x = dst_rect->x + pad; */
    /* dst_rectText->y = dst_rect->y + pad; */
    SDL_RenderCopy(renderer, texture, src_rect, dst_rect);
    SDL_Rect dst_rectText = {dst_rect->x + pad, dst_rect->y + pad, 5, 5};


    switch (state){
        case MOVING_DOWN:
            if (dst_rect->h <= 300){
                dst_rect->h += speed;
                src_rect->h += speed;
            }else{
                state = PAUSE_BOTTOM;
                pauseStart = SDL_GetTicks();
            }
            break;
        case PAUSE_BOTTOM:
            if (currentCharIndex <= textLength){
                char currentText[currentCharIndex + 2];
                strncpy(currentText, text, currentCharIndex + 1);
                currentText[currentCharIndex + 1] = '\0';
                SDL_Surface *surfaceText = TTF_RenderText_Blended_Wrapped(font, currentText, color, dst_rect->w - 50);
            SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
            SDL_FreeSurface(surfaceText);
            Uint32 format;
            int access, textWidth, textHeight;
            SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
            dst_rectText.h = textHeight ;
            dst_rectText.w = textWidth ;
            SDL_RenderCopy(renderer, textureText, NULL, &dst_rectText);
            SDL_DestroyTexture(textureText);
            if (SDL_GetTicks() - delayTimer >= delay){
                currentCharIndex ++;
                delayTimer = SDL_GetTicks();
            }
            /* SDL_DestroyTexture(textureText); */
            }else{
                state = SECOND_PAUSE;
                pauseStart = SDL_GetTicks();
            }
            break;
        case SECOND_PAUSE:
             SDL_Surface *surfaceText = TTF_RenderText_Blended_Wrapped(font, text, color, dst_rect->w - 50);
            SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
            SDL_FreeSurface(surfaceText);
            Uint32 format;
            int access, textWidth, textHeight;
            SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
            dst_rectText.h = textHeight ;
            dst_rectText.w = textWidth ;
            SDL_RenderCopy(renderer, textureText, NULL, &dst_rectText);

            if (SDL_GetTicks() - pauseStart >= interval){
                state = MOVING_UP;
            }
            break;

        case MOVING_UP:
            if (dst_rect->y >= 0){
                dst_rect->h -= speed;
                src_rect->h += speed;
            }
    }
}


