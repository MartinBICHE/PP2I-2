#ifndef dialog_h
#define dialog_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>


void render_sprite(SDL_Renderer *renderer, int x, int y, int scale, const char *image_path);
void print_string(const char *text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer, int x, int y, int scale);
char *show_time(Uint32 miliseconds);
void text_in_box(const char *text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer, int x, int y, int scale, int maxWidth);



#endif
