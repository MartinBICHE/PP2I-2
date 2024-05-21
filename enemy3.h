#ifndef enemy3_h
#define enemy3_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"

struct _Enemy3{
    float xPosition;
    float yPosition;
    int xCollisionMax;
    int xCollisionMin;
    int speed;
    int dx;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
};
typedef struct _Enemy3 Enemy3;
void initEnemy3(Enemy3 *enemy, int x, int y, int xCollisionMax);

void enemy3_movement(SDL_Renderer *renderer, Enemy3 *enemy, float x_cam);
#endif