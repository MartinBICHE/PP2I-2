#ifndef display_h
#define display_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>


void render_sprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int scale, const char *image_path);
void print_string(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int scale);
void text_in_box(const char *text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer, int x, int y, int scale, int maxWidth);

#endif
