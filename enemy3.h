#ifndef enemy3_h
#define enemy3_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "display.h"

struct _Enemy3{
    char *image_path;
    float xPosition;
    float yPosition;
    int xCollisionMax;
    int xCollisionMin;
    int speed;
};
typedef struct _Enemy3 Enemy3;
void enemy3_movement(SDL_Renderer *renderer, SDL_Texture *texture, Enemy3 *enemy, float scale);

#endif
