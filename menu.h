#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "const.h"

// Fonctions qui permettent de remettre le renderer à 0 
void closeLoadingWindow();
void closeMenuWindow();
void closeMapWindow();

// Fonctions qui permettent de dessiner les boutons 
void drawLoading();
void drawMenu();
void drawMapMenu();

// Fonctions qui touchent à la musique 
bool initSDL_mixer();
bool loadMusic();
void playMusic();
void toggleMusic();
void closeSDL_mixer();

// Fonctions qui permettent de changer la taille et le renderer de la fenêtre
bool initLoadingWindow();
bool initMenuWindow();
bool initPlayWindow();
void resizeWindow(SDL_Window*,int,int);

#endif /* MENU_H */