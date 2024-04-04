#ifndef CONST_H
#define CONST_H


#define HEIGHT 8 // i, y
#define WIDTH 48 // j, x
#define PIX_RECT 30
#define WINWIDTH WIDTH*PIX_RECT
#define WINHEIGHT HEIGHT*PIX_RECT

#define MENU_WINDOW_WIDTH 1400
#define MENU_WINDOW_HEIGHT 800
#define Image1Width 200
#define Image1Height 150
#define Image2Width 200
#define Image2Height 150
#define LOADING_WINDOW_WIDTH 600
#define LOADING_WINDOW_HEIGHT 400
#define BLUR_AMOUNT 0.5f
#define ImageRetourMenuWidth 200
#define ImageRetourMenuHeight 50
#define ImageQuitterJeuWidth 200
#define ImageQuitterJeuHeight 50

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
