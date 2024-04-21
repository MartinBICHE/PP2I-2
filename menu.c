#include "menu.h"

// Initialisation de toutes les variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Music* gMusic = NULL;
bool musicPaused = false;

// Fonction qui permet le redimensionnement de la fenêtre
void resizeWindow(SDL_Window* window, int width, int height) {
    SDL_SetWindowSize(window, width, height);
}

///////////////////////////////
//// Fenêtre de chargement ////
///////////////////////////////

// Fonction qui permet d'initaliser SDL, la fenêtre et le renderer
bool initLoadingWindow() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        return false;
    }
    // Création de la fenêtre
    window = SDL_CreateWindow("Fenêtre de chargement", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LOADING_WINDOW_WIDTH, LOADING_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Erreur lors de la création de la fenêtre de chargement : %s", SDL_GetError());
        return false;
    }
    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Erreur lors de la création du renderer de la fenêtre de chargement : %s", SDL_GetError());
        return false;
    }
    return true;
}

// Fonction qui supprime le renderer de la fenêtre de chargement 
void closeLoadingWindow() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
}

// Fonction qui dessine le renderer de la fenêtre de chargement
void drawLoading() {
    static int angle = 0;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    // Dessiner l'arrière-plan
    SDL_Surface* backgroundSurface = IMG_Load("./asset/background/Foret/menu-1.png");
    if (backgroundSurface == NULL) {
        SDL_Log("Erreur lors du chargement de l'image : %s", SDL_GetError());
        return;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture == NULL) {
        SDL_Log("Erreur lors de la création de la texture : %s", SDL_GetError());
        SDL_FreeSurface(backgroundSurface);
        return;
    }
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundSurface);
    // Dessiner le cercle avec des nuances de transparence
    int centerX = LOADING_WINDOW_WIDTH / 2;
    int centerY = LOADING_WINDOW_HEIGHT / 2;
    int radius = 30;
    int thickness = 3;
    for (int alpha = 255; alpha > 0; alpha -= 20) {
        SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, alpha);
        SDL_RenderDrawLine(renderer, centerX + (radius - thickness / 2) * cos(angle * M_PI / 180.0), centerY + (radius - thickness / 2) * sin(angle * M_PI / 180.0), centerX + (radius + thickness / 2) * cos((angle + 10) * M_PI / 180.0), centerY + (radius + thickness / 2) * sin((angle + 10) * M_PI / 180.0));
        angle += 3;
    }

    SDL_RenderPresent(renderer);
}

/////////////////////////
//// Fenêtre de menu ////
/////////////////////////

// Fonction qui change la taille de la fenêtre et qui réinitialise le renderer à cette nouvelle fenêtre
bool initMenuWindow() {
    resizeWindow(window, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Erreur lors de la création du renderer de la fenêtre de menu : %s", SDL_GetError());
        return false;
    }
    return true;
}

// Fonction qui supprime le renderer de la fenêtre de menu
void closeMenuWindow() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
}

// Fonction qui dessine le renderer de la fenêtre de menu
void drawMenu() {
    if (showMenu == true){
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Charger et dessiner l'arrière-plan
        SDL_Surface* backgroundSurface = IMG_Load("./asset/background/Foret/menu-2.jpg"); 
        if (backgroundSurface == NULL) {
            SDL_Log("Erreur lors du chargement de l'arrière-plan : %s", SDL_GetError());
            return;
        }
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        if (backgroundTexture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de l'arrière-plan : %s", SDL_GetError());
            SDL_FreeSurface(backgroundSurface);
            return;
        }
        SDL_Rect backgroundRect = {0, 0, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);

        // Charger et dessiner le bouton play
        SDL_Surface* image1Surface = IMG_Load("./asset/background/Foret/bouton-play.png"); 
        if (image1Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la première image : %s", SDL_GetError());
            return;
        }
        SDL_Texture* image1Texture = SDL_CreateTextureFromSurface(renderer, image1Surface);
        if (image1Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la première image : %s", SDL_GetError());
            SDL_FreeSurface(image1Surface);
            return;
        }
        SDL_Rect image1Rect = {(MENU_WINDOW_WIDTH - Image1Width) / 2, (MENU_WINDOW_HEIGHT - Image1Height) / 2, Image1Width, Image1Height};
        SDL_RenderCopy(renderer, image1Texture, NULL, &image1Rect);
        SDL_DestroyTexture(image1Texture);
        SDL_FreeSurface(image1Surface);

        // Charger et dessiner le bouton paramètre
        SDL_Surface* image2Surface = IMG_Load("./asset/background/Foret/bouton-paramètre.png");
        if (image2Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la deuxième image : %s", SDL_GetError());
            return;
        }
        SDL_Texture* image2Texture = SDL_CreateTextureFromSurface(renderer, image2Surface);
        if (image2Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la deuxième image : %s", SDL_GetError());
            SDL_FreeSurface(image2Surface);
            return;
        }
        SDL_Rect image2Rect = {MENU_WINDOW_WIDTH - Image2Width, 0, Image2Width, Image2Height};
        SDL_RenderCopy(renderer, image2Texture, NULL, &image2Rect);
        SDL_DestroyTexture(image2Texture);
        SDL_FreeSurface(image2Surface);

        // Charger et dessiner le boutton quitter le jeu
        SDL_Surface* image3Surface = IMG_Load("./asset/background/Foret/bouton-quitter-le-jeu.png");
        if (image2Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la deuxième image : %s", SDL_GetError());
            return;
        }
        SDL_Texture* image3Texture = SDL_CreateTextureFromSurface(renderer, image3Surface);
        if (image3Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la deuxième image : %s", SDL_GetError());
            SDL_FreeSurface(image3Surface);
            return;
        }

        SDL_Rect image3Rect = {0, 0, Image3Width, Image3Height};
        SDL_RenderCopy(renderer, image3Texture, NULL, &image3Rect);
        SDL_DestroyTexture(image3Texture);
        SDL_FreeSurface(image3Surface);
        SDL_RenderPresent(renderer);
    } else {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Charger et dessiner l'arrière-plan
        SDL_Surface* backgroundSurface = IMG_Load("./asset/background/Foret/menu-2.jpg"); 
        if (backgroundSurface == NULL) {
            SDL_Log("Erreur lors du chargement de l'arrière-plan : %s", SDL_GetError());
            return;
        }
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        if (backgroundTexture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de l'arrière-plan : %s", SDL_GetError());
            SDL_FreeSurface(backgroundSurface);
            return;
        }
        SDL_Rect backgroundRect = {0, 0, MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);

        // Dessiner une barre horizontale au milieu du curseur
        SDL_Rect barRect = {MENU_WINDOW_WIDTH / 4, MENU_WINDOW_HEIGHT / 2-5, MENU_WINDOW_WIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect);
        // Dessiner le curseur au milieu de la fenêtre
        SDL_Rect cursorRect = {(MENU_WINDOW_WIDTH - CURSOR_WIDTH) / 2, (MENU_WINDOW_HEIGHT - CURSOR_HEIGHT) / 2, CURSOR_WIDTH, CURSOR_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
        SDL_RenderPresent(renderer);
    }
}

/////////////////
//// Musique ////
/////////////////

// Fonction pour initialiser la musique
bool initSDL_mixer() {
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
        SDL_Log("Erreur lors de l'ouverture de l'audio : %s",Mix_GetError());
        return false;
    }
    return true;
}

// Fonction pour charger la musique
bool loadMusic() {
    gMusic = Mix_LoadMUS("./asset/musique/musique-menu.mp3");
    if (gMusic == NULL) {
        SDL_Log("Erreur lors du chargement de la musique : %s", Mix_GetError());
        return false;
    }
    return true;
}

// Fonction pour jouer la musique infiniment
void playMusic() {
    Mix_PlayMusic(gMusic, -1);
}

// Fonction pour libérer la musique et quitter SDL_mixer
void closeSDL_mixer() {
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    Mix_CloseAudio();
    Mix_Quit();
}

// Fonction pour mettre en pause ou reprendre la musique
void toggleMusic() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
        musicPaused = false;
    } else {
        Mix_PauseMusic();
        musicPaused = true;
    }
}

////////////////////////
//// Fenêtre de jeu ////
////////////////////////

// Fonction qui change la taille de la fenêtre et qui réinitialise le renderer à cette nouvelle fenêtre
bool initPlayWindow() {
    resizeWindow(window,WINWIDTH,WINHEIGHT);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log(SDL_LOG_CATEGORY_APPLICATION,"Erreur lors de la création du renderer de la fenêtre de jeu : %s", SDL_GetError());
        return false;
    }
    return true;
}

// Fonction qui dessine le renderer de la fenêtre de jeu
void drawMapMenu() {
    if (afficherImage) {

        // Charger et dessiner le boutton retout au menu
        SDL_Surface *image1Surface = IMG_Load("./asset/background/Foret/bouton-retour-menu.png");
        if (image1Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la première image : %s", SDL_GetError());
        }
        SDL_Texture *image1Texture = SDL_CreateTextureFromSurface(renderer, image1Surface);
        if (image1Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la première image : %s", SDL_GetError());
            SDL_FreeSurface(image1Surface);
        }
        SDL_Rect image1Rect = {(WINWIDTH - ImageRetourMenuWidth) / 2, (WINHEIGHT - ImageRetourMenuHeight) / 2 - 50, ImageRetourMenuWidth, ImageRetourMenuHeight};
        SDL_RenderCopy(renderer, image1Texture, NULL, &image1Rect);
        SDL_DestroyTexture(image1Texture);
        SDL_FreeSurface(image1Surface);

        // Charger et dessiner le boutton quitter le jeu
        SDL_Surface *image2Surface = IMG_Load("./asset/background/Foret/bouton-quitter-le-jeu.png");
        if (image2Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la première image : %s", SDL_GetError());
        }
        SDL_Texture *image2Texture = SDL_CreateTextureFromSurface(renderer, image2Surface);
        if (image1Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la première image : %s", SDL_GetError());
            SDL_FreeSurface(image2Surface);
        }
        SDL_Rect image2Rect = {(WINWIDTH - ImageQuitterJeuWidth) / 2, (WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50, ImageQuitterJeuWidth, ImageQuitterJeuHeight};
        SDL_RenderCopy(renderer, image2Texture, NULL, &image2Rect);
        SDL_DestroyTexture(image2Texture);
        SDL_FreeSurface(image2Surface);

        // Charger et dessiner le boutton paramètre
        SDL_Surface *image3Surface = IMG_Load("./asset/background/Foret/bouton-paramètre.png");
        if (image3Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la première image : %s", SDL_GetError());
        }
        SDL_Texture *image3Texture = SDL_CreateTextureFromSurface(renderer, image3Surface);
        if (image3Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la première image : %s", SDL_GetError());
            SDL_FreeSurface(image3Surface);
        }
        SDL_Rect image3Rect = {WINWIDTH - ImageParametrePauseWidth, 0, ImageParametrePauseWidth, ImageParametrePauseHeight};
        SDL_RenderCopy(renderer, image3Texture, NULL, &image3Rect);
        SDL_DestroyTexture(image3Texture);
        SDL_FreeSurface(image3Surface);

        // Charger et dessiner le boutton retour en arrière
        SDL_Surface *image4Surface = IMG_Load("./asset/background/Foret/bouton-retour-en-arrière.png");
        if (image4Surface == NULL) {
            SDL_Log("Erreur lors du chargement de la première image : %s", SDL_GetError());
        }
        SDL_Texture *image4Texture = SDL_CreateTextureFromSurface(renderer, image4Surface);
        if (image4Texture == NULL) {
            SDL_Log("Erreur lors de la création de la texture de la première image : %s", SDL_GetError());
            SDL_FreeSurface(image4Surface);
        }
        SDL_Rect image4Rect = {0, 0, ImageRetourArrièreWidth, ImageRetourArrièreHeight};
        SDL_RenderCopy(renderer, image4Texture, NULL, &image4Rect);
        SDL_DestroyTexture(image4Texture);
        SDL_FreeSurface(image4Surface);
    }

    if (parametre) {
        // Dessiner une barre horizontale au milieu du curseur
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 2-5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect); 

        // Dessiner le curseur au milieu de la fenêtre
        SDL_Rect cursorRect = {cursorX, cursorY, CURSOR_WIDTH, CURSOR_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
}

// Fonction qui supprime le renderer de la fenêtre de jeu
void closeMapWindow() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
}