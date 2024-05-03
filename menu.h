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

// Fonctions qui permettent de dessiner
void drawMenu();
void drawMapMenu();
void renderSprite();
void renderImage(const char* imagePath, int x, int y, int width, int height);

// Fonctions qui touchent à la musique 
bool initSDL_mixer();
bool loadMusic();
void playMusic();
void toggleMusic();
void closeSDL_mixer();

void interactionMenu();
void interactionPauseJeu();

#endif /* MENU_H */