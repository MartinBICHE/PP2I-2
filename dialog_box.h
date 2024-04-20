#ifndef dialog_box_h
#define dialog_box_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "const.h"

void render_box(SDL_Renderer *renderer, SDL_Texture *boxTexture, SDL_Texture *textTexture, int x, int y);
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color, SDL_Rect *dst_rect, SDL_Texture *boxTexture, TTF_Font *fontBold);

char *remove_first_word(const char *str);


#endif
