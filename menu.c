#include "menu.h"
#include "init.h"
#include "boss.h"

Mix_Music* gMusic1 = NULL;
Mix_Music* gMusic2 = NULL;
bool musicPaused = false;
int spriteIndex = 0;
int spriteScrollDelay = 300;
int volumeLevel = MAX_VOLUME / 2; // Volume initial à 50%
int sfxVolumeLevel = MAX_VOLUME / 2; // Volume initial pour les bruitages à 50%
SDL_Rect cursorRect;
SDL_Rect cursorRectSFX;
Mix_Chunk* sounds[4];

void initCursorRects(void) {
    cursorRect.x = WINWIDTH / 4 + (volumeLevel * (WINWIDTH / 2)) / MAX_VOLUME - CURSOR_WIDTH / 2;
    cursorRect.y = WINHEIGHT / 4 - CURSOR_HEIGHT / 2 ; // Volume de la musique
    cursorRect.w = CURSOR_WIDTH;
    cursorRect.h = CURSOR_HEIGHT;

    cursorRectSFX.x = WINWIDTH / 4 + (sfxVolumeLevel * (WINWIDTH / 2)) / MAX_VOLUME - CURSOR_WIDTH / 2;
    cursorRectSFX.y = WINHEIGHT / 4 * 3 - CURSOR_HEIGHT / 2; // Volume des bruitages
    cursorRectSFX.w = CURSOR_WIDTH;
    cursorRectSFX.h = CURSOR_HEIGHT;
}

void renderSprite(SDL_Renderer *renderer) {
    static Uint32 lastScrollTime = 0;
    Uint32 currentTime = SDL_GetTicks(); 
    SDL_Surface *spriteSurface = IMG_Load("asset/UI/BackgroundMenu.png");
    if (!spriteSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface init: %s", SDL_GetError());
        return;
    }
    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if (!spriteTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        SDL_FreeSurface(spriteSurface);
        return;
    }
    SDL_FreeSurface(spriteSurface);

    int imageWidth, imageHeight;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &imageWidth, &imageHeight);

    int spriteWidth = imageWidth / NUM_SPRITES;
    int spriteHeight = imageHeight;

    SDL_Rect spriteRect = {.x = spriteIndex * spriteWidth, .y = 0, .w = spriteWidth, .h = spriteHeight};
    
    if (showMenu) {
        if (currentTime - lastScrollTime >= spriteScrollDelay) {
            spriteIndex = (spriteIndex + 1) % NUM_SPRITES;
            lastScrollTime = currentTime; 
        }
    }

    SDL_Rect destRect = {.x = 0, .y = 0, .w = WINWIDTH, .h = WINHEIGHT};

    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
    }

    SDL_DestroyTexture(spriteTexture);
}

void renderImage(SDL_Renderer *renderer, const char* imagePath, int x, int y, int width, int height) {
    SDL_Surface* imageSurface = IMG_Load(imagePath);
    if (!imageSurface) {
        SDL_Log("Erreur lors du chargement de l'image : %s", SDL_GetError());
        return;
    }
    
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!imageTexture) {
        SDL_Log("Erreur lors de la création de la texture de l'image : %s", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        return;
    }
    
    SDL_Rect imageRect = {x, y, width, height};
    
    SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
    
    SDL_DestroyTexture(imageTexture);
    SDL_FreeSurface(imageSurface);
}

bool loadMusicGameplay1(void) {
    gMusic1 = Mix_LoadMUS("./asset/musique/gameplay 1.mp3");
    if (!gMusic1) {
        SDL_Log("Erreur lors du chargement de la musique : %s", Mix_GetError());
        return false;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    return true;
}

bool loadMusicGameplay2(void) {
    gMusic2 = Mix_LoadMUS("./asset/musique/gameplay 2.mp3");
    if (!gMusic2) {
        SDL_Log("Erreur lors du chargement de la musique : %s", Mix_GetError());
        return false;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    return true;
}

void playMusic(Mix_Music* music) {
    if (Mix_PlayMusic(music, -1) == -1) {
        SDL_Log("Erreur lors de la lecture de la musique : %s", Mix_GetError());
    }
}

void closeSDL_mixer(void) {
    if (gMusic1) {
        Mix_FreeMusic(gMusic1);
        gMusic1 = NULL;
    }
    if (gMusic2) {
        Mix_FreeMusic(gMusic2);
        gMusic2 = NULL;
    }
    Mix_CloseAudio();
    Mix_Quit();
}

void toggleMusic(void) {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
        musicPaused = false;
    } else {
        Mix_PauseMusic();
        musicPaused = true;
    }
}

void loadSounds(Mix_Chunk **sounds) {
    sounds[0] = Mix_LoadWAV("./asset/sounds/walk1.wav");
    if (sounds[0] == NULL) {
        printf("Failed to load walking sound 1 effect! SDL_mixer Error: %s\n", Mix_GetError());
        exit(-1);
    }
    sounds[1] = Mix_LoadWAV("./asset/sounds/walk2.wav");
    if (sounds[1] == NULL) {
        printf("Failed to load walking sound 2 effect! SDL_mixer Error: %s\n", Mix_GetError());
        exit(-1);
    }
    sounds[2] = Mix_LoadWAV("./asset/sounds/dash.wav");
    if (sounds[2] == NULL) {
        printf("Failed to load dashing sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        exit(-1);
    }
    sounds[3] = Mix_LoadWAV("./asset/sounds/true_crack.wav");
    if (sounds[3] == NULL) {
        printf("Failed to load crack sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        exit(-1);
    }
    
    // Appliquer le volume initial aux bruitages
    for (int i = 0; i < 4; i++) {
        Mix_VolumeChunk(sounds[i], sfxVolumeLevel);
    }
}


void handleVolumeCursor(SDL_Event *e) {
    if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION) {
        if (e->button.button == SDL_BUTTON_LEFT) {
            int mouseX = e->button.x;
            int mouseY = e->button.y;

            // Gérer le curseur de volume pour la musique
            if (mouseY >= WINHEIGHT / 4 - CURSOR_HEIGHT / 2 && mouseY <= WINHEIGHT / 4 + CURSOR_HEIGHT / 2) {
                cursorRect.x = mouseX - cursorRect.w / 2;
                if (cursorRect.x < WINWIDTH / 4) cursorRect.x = WINWIDTH / 4;
                if (cursorRect.x > WINWIDTH / 4 + WINWIDTH / 2 - cursorRect.w) cursorRect.x = WINWIDTH / 4 + WINWIDTH / 2 - cursorRect.w;
                volumeLevel = ((cursorRect.x - WINWIDTH / 4) * MAX_VOLUME) / (WINWIDTH / 2 - cursorRect.w);
                Mix_VolumeMusic(volumeLevel);
            }

            // Gérer le curseur de volume pour les bruitages
            if (mouseY >= WINHEIGHT / 4 * 3 - CURSOR_HEIGHT / 2 && mouseY <= WINHEIGHT / 4 * 3 + CURSOR_HEIGHT / 2) {
                cursorRectSFX.x = mouseX - cursorRectSFX.w / 2;
                if (cursorRectSFX.x < WINWIDTH / 4) cursorRectSFX.x = WINWIDTH / 4;
                if (cursorRectSFX.x > WINWIDTH / 4 + WINWIDTH / 2 - cursorRectSFX.w) cursorRectSFX.x = WINWIDTH / 4 + WINWIDTH / 2 - cursorRectSFX.w;
                sfxVolumeLevel = ((cursorRectSFX.x - WINWIDTH / 4) * MAX_VOLUME) / (WINWIDTH / 2 - cursorRectSFX.w);
                
                // Appliquer le volume à tous les bruitages
                for (int i = 0; i < 4; i++) {
                    Mix_VolumeChunk(sounds[i], sfxVolumeLevel);
                }
            }
        }
    }
}

void drawMenu(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    if (showMenu) {
        renderSprite(renderer);
        renderImage(renderer, "./asset/UI/play.png", (WINWIDTH / 2 - Image1Width / 2), (WINHEIGHT / 2 - Image1Height / 2), Image1Width, Image1Height);
        renderImage(renderer, "./asset/UI/option.png", WINWIDTH - ImageParametreMenuWidth, 0, ImageParametreMenuWidth, ImageParametreMenuHeight);
        renderImage(renderer, "./asset/UI/bouton-quitter-le-jeu.png", 0, 0, Image1Width, Image1Height-40);
    } else {
        renderSprite(renderer);
        renderImage(renderer,"./asset/UI/music.png",WINWIDTH / 4 - 200, WINHEIGHT / 4 - 75,150,150);
        renderImage(renderer,"./asset/UI/fx.png",WINWIDTH / 4 - 200, WINHEIGHT / 4 * 3 - 75,150,150);
        // Dessiner la barre de volume pour la musique
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 4 - 5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);

        // Dessiner la barre de volume pour les bruitages
        SDL_Rect barRectSFX = {WINWIDTH / 4, WINHEIGHT / 4 * 3 - 5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRectSFX);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &cursorRectSFX);
    }

    SDL_RenderPresent(renderer);
}

void drawMapMenu(SDL_Renderer *renderer) {
    if (afficherImage) {
        renderImage(renderer, "./asset/UI/bouton-retour-menu.png", (WINWIDTH - ImageRetourMenuWidth) / 2, (WINHEIGHT - ImageRetourMenuHeight) / 2 - 50, ImageRetourMenuWidth, ImageRetourMenuHeight);
        renderImage(renderer, "./asset/UI/bouton-quitter-le-jeu.png", (WINWIDTH - ImageQuitterJeuWidth) / 2, (WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50, ImageQuitterJeuWidth, ImageQuitterJeuHeight);
        renderImage(renderer, "./asset/UI/option.png", WINWIDTH - ImageParametrePauseWidth, 0, ImageParametrePauseWidth, ImageParametrePauseHeight);
        renderImage(renderer, "./asset/UI/bouton-retour-en-arrière.png", 0, 0, ImageRetourArrièreWidth, ImageRetourArrièreHeight);
        renderImage(renderer, "./asset/UI/bouton-quitter-le-jeu.png", (WINWIDTH - ImageQuitterJeuWidth) / 2, (WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50, ImageQuitterJeuWidth, ImageQuitterJeuHeight);
    }
    if (parametre) {
        renderImage(renderer,"./asset/UI/music.png",WINWIDTH / 4 - 200, WINHEIGHT / 4 - 75,150,150);
        renderImage(renderer,"./asset/UI/fx.png",WINWIDTH / 4 - 200, WINHEIGHT / 4 * 3 - 75,150,150);
        // Dessiner la barre de volume pour la musique
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 4 - 5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);

        // Dessiner la barre de volume pour les bruitages
        SDL_Rect barRectSFX = {WINWIDTH / 4, WINHEIGHT / 4 * 3 - 5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRectSFX);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &cursorRectSFX);
    }
}

void interactionMenu(SDL_Renderer *renderer) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= 0 && mouseX <= Image1Width && mouseY >= 0 && mouseY <= Image1Height - 40 && showMenu) {
                quit = true;
            } else if (mouseX >= ((WINWIDTH - Image1Width) / 2) && mouseX <= ((WINWIDTH - Image1Width) / 2) + Image1Width &&
                       mouseY >= ((WINHEIGHT - Image1Height) / 2) && mouseY <= ((WINHEIGHT - Image1Height) / 2) + Image1Height && showMenu) {
                startGame = true;
                playMusic(gMusic1);
            } else if (mouseX >= WINWIDTH - ImageParametreMenuWidth && mouseX <= WINWIDTH &&
                       mouseY >= 0 && mouseY <= ImageParametreMenuHeight && showMenu) {
                showMenu = false;
                parametre = !parametre;
            } else if (mouseX >= WINWIDTH - Image1Width && mouseX <= WINWIDTH &&
                       mouseY >= 0 && mouseY <= Image1Height && !showMenu) {
                showMenu = true;
            } else if (mouseX >= 0 && mouseX <= Image1Width && mouseY >= 0 && mouseY <= Image1Height && !showMenu) {
                showMenu = true;
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            handleVolumeCursor(&e); // Ajout du gestionnaire de curseur
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_ESCAPE && !showMenu) {
                showMenu = true;
                parametre = !parametre;
            }
        }
    }
    if (prevShowMenu != showMenu) {
        toggleMusic();
        prevShowMenu = showMenu;
    }
    drawMenu(renderer);
}

void interactionPauseJeu(SDL_Renderer *renderer) {
    if (e.type == SDL_QUIT) {
        quit = true;
    } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (afficherImage) {
            if (mouseX >= ((WINWIDTH - ImageQuitterJeuWidth) / 2) && mouseX <= ((WINWIDTH - ImageQuitterJeuWidth) / 2) + ImageQuitterJeuWidth &&
                mouseY >= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) && mouseY <= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) + ImageQuitterJeuHeight) {
                quit = true;
            } else if (mouseX >= ((WINWIDTH - ImageRetourMenuWidth) / 2) && mouseX <= ((WINWIDTH - ImageRetourMenuWidth) / 2) + ImageRetourMenuWidth &&
                       mouseY >= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) && mouseY <= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) + ImageRetourMenuHeight) {
                retourMenu = !retourMenu;
                toggleMusic();
                afficherImage = !afficherImage;
                isBossMap = false;
            } else if (mouseX >= (WINWIDTH - ImageParametrePauseWidth) && mouseX <= WINWIDTH &&
                       mouseY >= 0 && mouseY <= ImageParametrePauseHeight) {
                parametre = !parametre;
                afficherImage = false;
            } else if (mouseX >= 0 && mouseX <= ImageRetourArrièreWidth &&
                       mouseY >= 0 && mouseY <= ImageRetourArrièreHeight && afficherImage) {
                afficherImage = !afficherImage;
                toggleMusic();
            }
        }
    } else if (e.type == SDL_MOUSEMOTION) {
        handleVolumeCursor(&e); // Ajout du gestionnaire de curseur
    } else if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_ESCAPE && !afficherImage && !parametre) {
            afficherImage = !afficherImage;
            toggleMusic();
            parametre = false;
        } else if (e.key.keysym.sym == SDLK_ESCAPE && afficherImage) {
            afficherImage = !afficherImage;
            toggleMusic();
        } else if (e.key.keysym.sym == SDLK_ESCAPE && parametre) {
            parametre = !parametre;
            afficherImage = !afficherImage;
        }
    }
}

void resetGame(SDL_Window **window, SDL_Renderer **renderer, Map **map, Perso **perso, Boss **boss) {
    // Nettoyez les ressources existantes
    cleanupProjectiles();
    freeProjectileTexture();
    closeSDL_mixer();
    free(*perso);
    destroyMap(*map); 
    free(*boss);

    // Réinitialisez les variables globales
    running = true;
    showMenu = true;
    parametre = false;
    afficherImage = false;
    retourMenu = false;
    startGame = false;
    prevShowMenu = true;
    musicToggled = false;
    currentGravity = ACC;
    jumpSpeed = JUMPSPEED;
    showAttentionImage = true;

    // Réinitialisez les projectiles
    resetProjectiles();

    // Initialiser la carte, le personnage et le boss en fonction de la valeur de isBossMap
    if (isBossMap) {
        *map = initMap("mapBoss1");
    } else {
        *map = initMap("map2");
    }

    *perso = create_perso(*map);
}


void gameOver1(SDL_Renderer * renderer, SDL_Texture *bgTextures[], int layer, Map *map) { // cette fonction joue une "cinématique" de game over et est joué entre les gameplay 1 et 2 quand le perso meurt
    printf("game over\n");
    SDL_Surface *gameOverSurface = IMG_Load("asset/aseprite/Sprite-0002.png");
    if (!gameOverSurface){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init game over surface : %s", SDL_GetError());
		exit(-1);
	}
    SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    if (!gameOverTexture){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init game over texture : %s", SDL_GetError());
		exit(-1);
	}

    int n1 = 60; // nombre de frames de la cinématique
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
    for (int i = 0; i < n1; i++) {
        drawBackground(renderer, bgTextures, layer, map);
        SDL_SetTextureAlphaMod(gameOverTexture, (i*255)/n1);
        SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }


    SDL_FreeSurface(gameOverSurface);
    SDL_DestroyTexture(gameOverTexture);
}
