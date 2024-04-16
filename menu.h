#ifndef MENU_H
#define MENU_H

// Déclaration des fonctions pour le menu
bool initLoadingWindow();
void closeLoadingWindow();
void drawLoading();
bool initMenuWindow();
void closeMenuWindow();
void drawMenu();
bool initSDL_mixer();
bool loadMusic();
void playMusic();
void closeSDL_mixer();
void toggleMusic();
void drawMap();
bool initPlayWindow();

#endif /* MENU_H */