#ifndef dialog_box_h
#define dialog_box_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "const.h"


struct _DialogBoxData{
    int currentCharIndex;
    Uint32 delayTimer;
    int counter;
    SDL_Rect dst_rect;
};
typedef struct _DialogBoxData DialogBoxData;

char *remove_first_word(const char *str);

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                  SDL_Color color, DialogBoxData *dialogBoxData, 
                 SDL_Texture *boxTexture, TTF_Font *fontBold);

void initPapirus(DialogBoxData *dialogBoxData, int x, int y);

#endif
