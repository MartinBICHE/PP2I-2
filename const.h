#ifndef CONST_H
#define CONST_H

#include <stdbool.h>

extern bool showMenu;
extern bool afficherImage;
extern bool parametre;

extern SDL_Window* gLoadingWindow;
extern SDL_Renderer* gLoadingRenderer;

extern SDL_Window* gMenuWindow;
extern SDL_Renderer* gMenuRenderer;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

#define cursorX WINWIDTH / 2 - CURSOR_WIDTH / 2
#define cursorY WINHEIGHT / 2 - CURSOR_HEIGHT / 2
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
#define Image3Width 200
#define Image3Height 150
#define LOADING_WINDOW_WIDTH 600
#define LOADING_WINDOW_HEIGHT 400
#define BLUR_AMOUNT 0.5f
#define ImageRetourMenuWidth 200
#define ImageRetourMenuHeight 50
#define ImageQuitterJeuWidth 200
#define ImageQuitterJeuHeight 50
#define ImageParametrePauseWidth 50
#define ImageParametrePauseHeight 50
#define ImageRetourArrièreWidth 50
#define ImageRetourArrièreHeight 50

// Taille du curseur
#define CURSOR_WIDTH 20
#define CURSOR_HEIGHT 40

// Coordonnées du curseur
// #define cursorX WINWIDTH / 2 - CURSOR_WIDTH / 2;
// #define cursorY WINHEIGHT / 2 - CURSOR_HEIGHT / 2;

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
