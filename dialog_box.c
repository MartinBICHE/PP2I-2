#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "dialog_box.h"


void render_box(SDL_Renderer *renderer, SDL_Texture *boxTexture, SDL_Texture *textTexture, int x, int y){
    const int pad = 20;
    if (!boxTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }
    if (!textTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }
    Uint32 format;
    int access, textWidth, textHeight;
    SDL_QueryTexture(textTexture, &format, &access, &textWidth, &textHeight);
    SDL_Rect dstRectBox = {x, y, textWidth + 2*pad, textHeight + 2*pad};
    SDL_Rect dstRectText = {x + pad, y + pad , textWidth, textHeight};
    SDL_RenderCopy(renderer, boxTexture, NULL, &dstRectBox);
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRectText);


/* à faire une fonction qui fait séparement les RenderCopy */


}
