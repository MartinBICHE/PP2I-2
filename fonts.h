
#ifndef FONTS_H
#define FONTS_H
#include <SDL2/SDL_ttf.h>

extern TTF_Font* fontDialogBox;
extern TTF_Font* fontDialogBoxBold;
extern TTF_Font* fontScroll;

int loadFonts(void);
int freeFonts(void);
#endif
