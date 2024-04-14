#ifndef CONST_H
#define CONST_H


#define HEIGHT 8 // i, y
#define WIDTH 48 // j, x
#define PIX_RECT 30
#define WINWIDTH WIDTH*PIX_RECT
#define WINHEIGHT HEIGHT*PIX_RECT
#define TIERWIDTH WINWIDTH/3
#define SEMIHEIGHT WINHEIGHT/2
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
