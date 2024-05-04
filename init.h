#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "const.h"

void initSDL(SDL_Window **window, SDL_Renderer **renderer);
void quitSDL(SDL_Renderer **renderer, SDL_Window **window,  Perso *perso, Map *map);