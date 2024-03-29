#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "menu.h"
#include "const.h"

// Taille de la fenêtre de chargement

SDL_Window* gLoadingWindow = NULL;
SDL_Renderer* gLoadingRenderer = NULL;

SDL_Window* gMenuWindow = NULL;
SDL_Renderer* gMenuRenderer = NULL;

// Déclaration de la musique
Mix_Music* gMusic = NULL;

// Fonction pour initialiser SDL et créer la fenêtre de chargement
bool initLoadingWindow() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        return false;
    }

    // Création de la fenêtre de chargement
    gLoadingWindow = SDL_CreateWindow("Fenêtre de chargement", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LOADING_WINDOW_WIDTH, LOADING_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (gLoadingWindow == NULL) {
        SDL_Log("Erreur lors de la création de la fenêtre de chargement : %s", SDL_GetError());
        return false;
    }

    // Création du renderer de la fenêtre de chargement
    gLoadingRenderer = SDL_CreateRenderer(gLoadingWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gLoadingRenderer == NULL) {
        SDL_Log("Erreur lors de la création du renderer de la fenêtre de chargement : %s", SDL_GetError());
        return false;
    }

    return true;
}

// Fonction pour nettoyer et quitter SDL
void closeLoadingWindow() {
    SDL_DestroyRenderer(gLoadingRenderer);
    SDL_DestroyWindow(gLoadingWindow);
    gLoadingWindow = NULL;
    gLoadingRenderer = NULL;

    SDL_Quit();
}

// Fonction pour dessiner le motif de chargement (un cercle avec des nuances de transparence)
void drawLoading() {
    static int angle = 0;
    SDL_SetRenderDrawColor(gLoadingRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gLoadingRenderer);

    // Dessiner l'arrière-plan
    SDL_Surface* backgroundSurface = IMG_Load("./asset/background/Foret/menu-1.png");
    if (backgroundSurface == NULL) {
        SDL_Log("Erreur lors du chargement de l'image : %s", SDL_GetError());
        return;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(gLoadingRenderer, backgroundSurface);
    if (backgroundTexture == NULL) {
        SDL_Log("Erreur lors de la création de la texture : %s", SDL_GetError());
        SDL_FreeSurface(backgroundSurface);
        return;
    }
    SDL_RenderCopy(gLoadingRenderer, backgroundTexture, NULL, NULL);
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundSurface);

    // Dessiner le cercle avec des nuances de transparence
    int centerX = LOADING_WINDOW_WIDTH / 2;
    int centerY = LOADING_WINDOW_HEIGHT / 2;
    int radius = 30;
    int thickness = 3;
    for (int alpha = 255; alpha > 0; alpha -= 20) {
        SDL_SetRenderDrawColor(gLoadingRenderer, 0x80, 0x80, 0x80, alpha);
        SDL_RenderDrawLine(gLoadingRenderer, centerX + (radius - thickness / 2) * cos(angle * M_PI / 180.0), centerY + (radius - thickness / 2) * sin(angle * M_PI / 180.0), centerX + (radius + thickness / 2) * cos((angle + 10) * M_PI / 180.0), centerY + (radius + thickness / 2) * sin((angle + 10) * M_PI / 180.0));
        angle += 3;
    }

    SDL_RenderPresent(gLoadingRenderer);
}

// Fonction pour initialiser SDL et créer la fenêtre de menu en plein écran
bool initMenuWindow() {
    // Création de la fenêtre de menu en mode plein écran
    gMenuWindow = SDL_CreateWindow("Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (gMenuWindow == NULL) {
        SDL_Log("Erreur lors de la création de la fenêtre de menu : %s", SDL_GetError());
        return false;
    }

    // Création du renderer de la fenêtre de menu
    gMenuRenderer = SDL_CreateRenderer(gMenuWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gMenuRenderer == NULL) {
        SDL_Log("Erreur lors de la création du renderer de la fenêtre de menu : %s", SDL_GetError());
        return false;
    }

    return true;
}

// Fonction pour nettoyer et quitter SDL
void closeMenuWindow() {
    SDL_DestroyRenderer(gMenuRenderer);
    SDL_DestroyWindow(gMenuWindow);
    gMenuWindow = NULL;
    gMenuRenderer = NULL;
}

// Fonction pour dessiner le menu avec un arrière-plan et des images cliquables
void drawMenu(int image1Width, int image1Height, int image2Width, int image2Height) {
    // Effacer l'écran
    SDL_SetRenderDrawColor(gMenuRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gMenuRenderer);

    // Charger et dessiner l'arrière-plan
    SDL_Surface* backgroundSurface = IMG_Load("./asset/background/Foret/menu-2.jpg"); 
    if (backgroundSurface == NULL) {
        SDL_Log("Erreur lors du chargement de l'arrière-plan : %s", SDL_GetError());
        return;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(gMenuRenderer, backgroundSurface);
    if (backgroundTexture == NULL) {
        SDL_Log("Erreur lors de la création de la texture de l'arrière-plan : %s", SDL_GetError());
        SDL_FreeSurface(backgroundSurface);
        return;
    }
    SDL_Rect backgroundRect = {0, 0, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT};
    SDL_RenderCopy(gMenuRenderer, backgroundTexture, NULL, &backgroundRect);
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundSurface);

    // Charger et dessiner la première image au centre de la fenêtre
    SDL_Surface* image1Surface = IMG_Load("./asset/background/Foret/bouton-play.png"); 
    if (image1Surface == NULL) {
        SDL_Log("Erreur lors du chargement de la première image : %s", SDL_GetError());
        return;
    }
    SDL_Texture* image1Texture = SDL_CreateTextureFromSurface(gMenuRenderer, image1Surface);
    if (image1Texture == NULL) {
        SDL_Log("Erreur lors de la création de la texture de la première image : %s", SDL_GetError());
        SDL_FreeSurface(image1Surface);
        return;
    }
    SDL_Rect image1Rect = {(MENU_WINDOW_WIDTH - image1Width) / 2, (MENU_WINDOW_HEIGHT - image1Height) / 2, image1Width, image1Height};
    SDL_RenderCopy(gMenuRenderer, image1Texture, NULL, &image1Rect);
    SDL_DestroyTexture(image1Texture);
    SDL_FreeSurface(image1Surface);

    // Charger et dessiner la deuxième image en haut à droite de la fenêtre
    SDL_Surface* image2Surface = IMG_Load("./asset/background/Foret/bouton-paramètre.png");
    if (image2Surface == NULL) {
        SDL_Log("Erreur lors du chargement de la deuxième image : %s", SDL_GetError());
        return;
    }
    SDL_Texture* image2Texture = SDL_CreateTextureFromSurface(gMenuRenderer, image2Surface);
    if (image2Texture == NULL) {
        SDL_Log("Erreur lors de la création de la texture de la deuxième image : %s", SDL_GetError());
        SDL_FreeSurface(image2Surface);
        return;
    }
    SDL_Rect image2Rect = {MENU_WINDOW_WIDTH - image2Width, 0, image2Width, image2Height};
    SDL_RenderCopy(gMenuRenderer, image2Texture, NULL, &image2Rect);
    SDL_DestroyTexture(image2Texture);
    SDL_FreeSurface(image2Surface);

    // Mettre à jour l'affichage
    SDL_RenderPresent(gMenuRenderer);
}

// Fonction pour initialiser SDL_mixer
bool initSDL_mixer() {
    // Initialisation de SDL_mixer
    if (Mix_Init(MIX_INIT_MP3) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL_mixer : %s", Mix_GetError());
        return false;
    }

    // Initialisation de l'audio
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
        SDL_Log("Erreur lors de l'ouverture de l'audio : %s",Mix_GetError());
        return false;
    }

    return true;
}

// Fonction pour charger la musique
bool loadMusic() {
    // Charger la musique
    gMusic = Mix_LoadMUS("./asset/musique/musique-menu.mp3"); // Remplacez "musique.mp3" par le nom de votre musique
    if (gMusic == NULL) {
        SDL_Log("Erreur lors du chargement de la musique : %s", Mix_GetError());
        return false;
    }

    return true;
}

// Fonction pour jouer la musique
void playMusic() {
    // Jouer la musique en boucle (-1 indique une lecture infinie)
    Mix_PlayMusic(gMusic, -1);
}

// Fonction pour libérer la musique et quitter SDL_mixer
void closeSDL_mixer() {
    Mix_FreeMusic(gMusic);
    gMusic = NULL;

    Mix_CloseAudio();
    Mix_Quit();
}
