#ifndef CONST_H
#define CONST_H


<<<<<<< HEAD
#define WINHEIGHT 720 // en pixels
#define WINWIDTH 1280 // en pixels
#define HEIGHT 8 // i, y en nombre de tiles
#define WIDTH 48 // j, x en nombre de tiles
#define PIX_RECT WINHEIGHT/HEIGHT // en pixels
#define LEVEL_WIDTH PIX_RECT*WIDTH // en pixels

#define PERSO_WIDTH 60
#define PERSO_HEIGHT 60
#define ACC 25.5f
#define DT 1/60.0f
#define JUMPSPEED 11.0f
#define MOOVSPEED 7.0f
#define ANIMATION_FRAME_RATE 12
#define HEIGHT 8 // i, y
#define WIDTH 48 // j, x
#define PIX_RECT 30
#define GRAVITY 10
#define WINWIDTH WIDTH*PIX_RECT
#define WINHEIGHT HEIGHT*PIX_RECT


typedef struct _Map {
    char matrix[HEIGHT][WIDTH] ;
    float start_x ;
    float start_y ;
} Map ;


typedef struct _Perso {
    float x; // en nombre de tiles
    float y; // en nombre de tiles
    float vx; // en nombre de tiles par seconde
    float vy; // en nombre de tiles par seconde
    SDL_Rect hitbox;
    int health; // Le type peut changer, à voir comment la santé sera définie
} Perso;


#endif
