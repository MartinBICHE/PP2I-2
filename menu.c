#include "menu.h"
#include "init.h"

Mix_Music* gMusic = NULL;
bool musicPaused = false;
int spriteIndex = 0;
int spriteScrollDelay = 300;

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

bool initSDL_mixer(void) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Erreur lors de l'ouverture de l'audio : %s", Mix_GetError());
        return false;
    }
    return true;
}

bool loadMusic(void) {
    gMusic = Mix_LoadMUS("./asset/musique/musique-menu.mp3");
    if (!gMusic) {
        SDL_Log("Erreur lors du chargement de la musique : %s", Mix_GetError());
        return false;
    }
    return true;
}

void playMusic(void) {
    Mix_PlayMusic(gMusic, -1);
}

void closeSDL_mixer(void) {
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
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

void drawMenu(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    if (showMenu) {
        renderSprite(renderer);
        renderImage(renderer, "./asset/UI/play.png", (WINWIDTH / 2 - Image1Width / 2), (WINHEIGHT / 2 - Image1Height / 2), Image1Width, Image1Height);
        renderImage(renderer, "./asset/UI/option.png", WINWIDTH - Image1Width, 0, Image1Width, Image1Height);
        renderImage(renderer, "./asset/UI/bouton-quitter-le-jeu.png", 0, 0, Image1Width, Image1Height);
        SDL_RenderPresent(renderer);
    } else {
        renderSprite(renderer);
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 2 - 5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect);
        SDL_Rect cursorRect = {(WINWIDTH - CURSOR_WIDTH) / 2, (WINHEIGHT - CURSOR_HEIGHT) / 2, CURSOR_WIDTH, CURSOR_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
        SDL_RenderPresent(renderer);
    }
}

void drawMapMenu(SDL_Renderer *renderer) {
    if (afficherImage) {
        renderImage(renderer, "./asset/UI/bouton-retour-menu.png", (WINWIDTH - ImageRetourMenuWidth) / 2, (WINHEIGHT - ImageRetourMenuHeight) / 2 - 50, ImageRetourMenuWidth, ImageRetourMenuHeight);
        renderImage(renderer, "./asset/UI/bouton-quitter-le-jeu.png", (WINWIDTH - ImageQuitterJeuWidth) / 2, (WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50, ImageQuitterJeuWidth, ImageQuitterJeuHeight);
        renderImage(renderer, "./asset/UI/option.png", WINWIDTH - ImageParametrePauseWidth, 0, ImageParametrePauseWidth, ImageParametrePauseHeight);
        renderImage(renderer, "./asset/UI/bouton-retour-en-arrière.png", 0, 0, ImageRetourArrièreWidth, ImageRetourArrièreHeight);
    }
    if (parametre) {
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 2 - 5, WINWIDTH / 2, 10};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &barRect);
        SDL_Rect cursorRect = {(WINWIDTH - CURSOR_WIDTH) / 2, (WINHEIGHT - CURSOR_HEIGHT) / 2, CURSOR_WIDTH, CURSOR_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
}

void interactionMenu(SDL_Renderer *renderer) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= 0 && mouseX <= Image1Width && mouseY >= 0 && mouseY <= Image1Height && showMenu) {
                quit = true;
            } else if (mouseX >= ((WINWIDTH - Image1Width) / 2) && mouseX <= ((WINWIDTH - Image1Width) / 2) + Image1Width &&
                       mouseY >= ((WINHEIGHT - Image1Height) / 2) && mouseY <= ((WINHEIGHT - Image1Height) / 2) + Image1Height && showMenu) {
                startGame = true;
            } else if (mouseX >= WINWIDTH - Image1Width && mouseX <= WINWIDTH &&
                       mouseY >= 0 && mouseY <= Image1Height && showMenu) {
                showMenu = false;
            } else if (mouseX >= ((WINWIDTH - ImageRetourMenuWidth) / 2) && mouseX <= ((WINWIDTH - ImageRetourMenuWidth) / 2) + ImageRetourMenuWidth &&
                       mouseY >= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) && mouseY <= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) + ImageRetourMenuHeight) {
                showMenu = true;
            } else if (mouseX >= ((WINWIDTH - ImageQuitterJeuWidth) / 2) && mouseX <= ((WINWIDTH - ImageQuitterJeuWidth) / 2) + ImageQuitterJeuWidth &&
                       mouseY >= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) && mouseY <= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) + ImageQuitterJeuHeight) {
                quit = true;
            } else if (mouseX >= WINWIDTH - Image1Width && mouseX <= WINWIDTH &&
                       mouseY >= 0 && mouseY <= Image1Height && !showMenu) {
                showMenu = true;
            } else if (mouseX >= 0 && mouseX <= Image1Width && mouseY >= 0 && mouseY <= Image1Height && !showMenu) {
                showMenu = true;
            } else if (mouseX >= (WINWIDTH - ImageParametrePauseWidth) && mouseX <= WINWIDTH &&
                       mouseY >= 0 && mouseY <= ImageParametrePauseHeight) {
                parametre = !parametre;
                afficherImage = false;
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            if (e.motion.x >= (WINWIDTH - Image1Width) / 2 && e.motion.x <= ((WINWIDTH - Image1Width) / 2) + Image1Width &&
                e.motion.y >= (WINHEIGHT - Image1Height) / 2 && e.motion.y <= ((WINHEIGHT - Image1Height) / 2) + Image1Height) {
                spriteScrollDelay = 100;
            } else {
                spriteScrollDelay = 300;
            }
            if (e.motion.x >= (WINWIDTH - ImageQuitterJeuWidth) / 2 && e.motion.x <= ((WINWIDTH - ImageQuitterJeuWidth) / 2) + ImageQuitterJeuWidth &&
                e.motion.y >= (WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50 && e.motion.y <= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) + ImageQuitterJeuHeight) {
                showMenu = true;
            } else if (e.motion.x >= (WINWIDTH - ImageRetourMenuWidth) / 2 && e.motion.x <= ((WINWIDTH - ImageRetourMenuWidth) / 2) + ImageRetourMenuWidth &&
                       e.motion.y >= (WINHEIGHT - ImageRetourMenuHeight) / 2 - 50 && e.motion.y <= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) + ImageRetourMenuHeight) {
                showMenu = true;
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_ESCAPE && !showMenu) {
                showMenu = true;
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
    *perso = NULL;
    *map = NULL;
    *boss = NULL;

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

    if (!loadProjectileTexture(*renderer)) {
        SDL_Log("Erreur lors du chargement de la texture du projectile.");
        exit(1);
    }

    // Initialiser SDL_mixer et charger la musique
    if (!initSDL_mixer()) {
        SDL_Log("Erreur lors de l'initialisation de SDL_mixer.");
        exit(1);
    }

    if (!loadMusic()) {
        SDL_Log("Erreur lors du chargement de la musique.");
        closeSDL_mixer();
        exit(1);
    }

    // Jouer la musique du menu
    playMusic();

    // Initialiser la carte, le personnage et le boss en fonction de la valeur de isBossMap
    if (isBossMap) {
        *map = initMap("mapBoss1");
    } else {
        *map = initMap("map2");
    }

    *perso = create_perso(*map);
    if (isBossMap) {
        *boss = create_boss(*map);
    }
}
