#ifndef MENU_H
#define MENU_H
#include <stdbool.h>

// Déclaration des fonctions pour le menu
bool initLoadingWindow();
void closeLoadingWindow();
void drawLoading();
bool initMenuWindow();
void closeMenuWindow();
void drawMenu(int image1Width, int image1Height, int image2Width, int image2Height);
bool initSDL_mixer();
bool loadMusic();
void playMusic();
void closeSDL_mixer();

#endif /* MENU_H */
