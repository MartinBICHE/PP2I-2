#ifndef CONST_H
#define CONST_H


#define WINHEIGHT 720
#define WINWIDTH 1280
#define HEIGHT 8 // i, y
#define WIDTH 48 // j, x
#define PIX_RECT WINHEIGHT/HEIGHT


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
