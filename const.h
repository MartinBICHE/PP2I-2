#ifndef CONST_H
#define CONST_H

#define WINHEIGHT 720 // en pixels
#define WINWIDTH 1280 // en pixels
#define HEIGHT 8 // i, y en nombre de tiles
#define WIDTH 48 // j, x en nombre de tiles
#define PIX_RECT WINHEIGHT/HEIGHT // en pixels
#define LEVEL_WIDTH PIX_RECT*WIDTH // en pixels
#define TIERWIDTH WINWIDTH/3
#define QUARTERHEIGHT WINHEIGHT/4
#define SPRITESIZE 64 // 64x64 taille des sprites de base

typedef struct _Map {
    char matrix[HEIGHT][WIDTH] ;
    float start_x ;
    float start_y ;
} Map ;

typedef struct _Perso {
    int x ;
    int y ;
    int vx ;
    int vy ;
} Perso ;

#endif
