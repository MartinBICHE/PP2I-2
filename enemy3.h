#ifndef enemy3_h
#define enemy3_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#define INIT_ENEMY3(enemy, map, x, y, xMax) initEnemy3(enemy, (x) * (map)->pix_rect, (y) * (map)->pix_rect, (xMax) * (map)->pix_rect)


enum Enemy3State{
    RIGHT,
    LEFT,
    ATTACK
};

struct _Enemy3{
    enum Enemy3State state;
    float xPosition;
    float yPosition;
    int xCollisionMax;
    int xCollisionMin;
    int speed;
    int dx;
    SDL_Rect src_rect;
    SDL_Rect src_rectAttack;
    SDL_Rect dst_rect;
    Uint32 pauseAttack;
    Uint32 pauseMusic;
    Uint32 pauseBits;
    enum Enemy3State previousState;
    int health;
};
typedef struct _Enemy3 Enemy3;
void initEnemy3(Enemy3 *enemy, int x, int y, int xCollisionMax);

void enemy3Attack(Enemy3 *enemy, Perso *perso, Map *map);
void enemy3_movement(SDL_Renderer *renderer, Enemy3 *enemy, Map *map);
void updatePersoEnemy3(Perso *perso, Map *map, Enemy3 *enemy);
#endif
