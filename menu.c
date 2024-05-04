#include "menu.h"

Mix_Music* gMusic = NULL;

bool musicPaused = false;
int spriteIndex = 0;
int spriteScrollDelay = 300;

void renderSprite() {
    static Uint32 lastScrollTime = 0;
    Uint32 currentTime = SDL_GetTicks(); 
    SDL_Surface *spriteSurface = IMG_Load("asset/UI/BackgroundMenu.png");
    if (!spriteSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in sprite surface init: %s",SDL_GetError());
    }
    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if (!spriteTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in spritetexture inti: %s",SDL_GetError());
    }
    SDL_FreeSurface(spriteSurface);

    int imageWidth, imageHeight;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &imageWidth, &imageHeight);

    int spriteWidth = imageWidth / NUM_SPRITES;
    int spriteHeight = imageHeight;

    SDL_Rect spriteRect = {.x = spriteIndex * spriteWidth, .y = 0, .w = spriteWidth, .h = spriteHeight};
    
    if (showMenu == true){
        if (currentTime - lastScrollTime >= spriteScrollDelay) {
            if (spriteIndex == 7) {
                spriteIndex = 0;
            } else {
                spriteIndex++;
            }
            lastScrollTime = currentTime; 
        }
    }

    SDL_Rect destRect = {.x = 0, .y = 0, .w = WINWIDTH, .h = WINHEIGHT};

    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in render copy: %s",SDL_GetError());
    }

    SDL_DestroyTexture(spriteTexture);
}

void renderImage(const char* imagePath, int x, int y, int width, int height) {
    SDL_Surface* imageSurface = IMG_Load(imagePath);
    if (imageSurface == NULL) {
        SDL_Log("Erreur lors du chargement de l'image : %s", SDL_GetError());
        return;
    }
    
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (imageTexture == NULL) {
        SDL_Log("Erreur lors de la création de la texture de l'image : %s", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        return;
    }
    
    SDL_Rect imageRect = {x, y, width, height};
    
    SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
    
    SDL_DestroyTexture(imageTexture);
    SDL_FreeSurface(imageSurface);
}

bool initSDL_mixer() {
    if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0) {
        SDL_Log("Erreur lors de l'ouverture de l'audio : %s",Mix_GetError());
        return false;
    }
    return true;
}

bool loadMusic() {
    gMusic = Mix_LoadMUS("./asset/musique/musique-menu.mp3");
    if (gMusic == NULL) {
        SDL_Log("Erreur lors du chargement de la musique : %s", Mix_GetError());
        return false;
    }
    return true;
}

void playMusic() {
    Mix_PlayMusic(gMusic, -1);
}

void closeSDL_mixer() {
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    Mix_CloseAudio();
    Mix_Quit();
}

void toggleMusic() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
        musicPaused = false;
    } else {
        Mix_PauseMusic();
        musicPaused = true;
    }
}

void drawMenu() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    if (showMenu == true){
        renderSprite();
        renderImage("./asset/UI/play.png",(WINWIDTH / 2 - Image1Width / 2),(WINHEIGHT / 2 - Image1Height / 2), Image1Width, Image1Height);
        renderImage("./asset/UI/option.png", WINWIDTH - Image2Width, 0, Image2Width, Image2Height);
        renderImage("./asset/background/Foret/bouton-quitter-le-jeu.png",0, 0, Image3Width, Image3Height);
        SDL_RenderPresent(renderer);
    } else {
        renderSprite();
                                                                                                                        //////
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 2-5, WINWIDTH / 2, 10};                                               //
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                                                                     //
        SDL_RenderFillRect(renderer, &barRect);                                                                             //-- va être changer don't worry c'est juste pour qu'on remarque que le bouton paramètre marche
        SDL_Rect cursorRect = {(WINWIDTH - CURSOR_WIDTH) / 2, (WINHEIGHT - CURSOR_HEIGHT) / 2, CURSOR_WIDTH, CURSOR_HEIGHT};//
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                                                                   //
        SDL_RenderFillRect(renderer, &cursorRect);                                                                      //////

        SDL_RenderPresent(renderer);
    }
}

void drawMapMenu() {
    if (afficherImage) {
        renderImage("./asset/background/Foret/bouton-retour-menu.png",(WINWIDTH - ImageRetourMenuWidth) / 2, (WINHEIGHT - ImageRetourMenuHeight) / 2 - 50, ImageRetourMenuWidth, ImageRetourMenuHeight);
        renderImage("./asset/background/Foret/bouton-quitter-le-jeu.png",(WINWIDTH - ImageQuitterJeuWidth) / 2, (WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50, ImageQuitterJeuWidth, ImageQuitterJeuHeight);
        renderImage("./asset/UI/option.png",WINWIDTH - ImageParametrePauseWidth, 0, ImageParametrePauseWidth, ImageParametrePauseHeight);
        renderImage("./asset/background/Foret/bouton-retour-en-arrière.png",0, 0, ImageRetourArrièreWidth, ImageRetourArrièreHeight);
    }
    if (parametre) {
                                                                                                                        //////
        SDL_Rect barRect = {WINWIDTH / 4, WINHEIGHT / 2-5, WINWIDTH / 2, 10};                                               //
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);                                                                     //
        SDL_RenderFillRect(renderer, &barRect);                                                                             //-- va être changer don't worry c'est juste pour qu'on remarque que le bouton paramètre marche
        SDL_Rect cursorRect = {(WINWIDTH - CURSOR_WIDTH) / 2, (WINHEIGHT - CURSOR_HEIGHT) / 2, CURSOR_WIDTH, CURSOR_HEIGHT};//
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                                                                   //
        SDL_RenderFillRect(renderer, &cursorRect);                                                                      //////
    }
}

void interactionMenu() {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
                if (mouseX >= 0 && mouseX <= Image3Width && mouseY >= 0 && mouseY <= Image3Height && showMenu == true) {
                    quit = true;
                } else if (mouseX >= ((WINWIDTH - Image1Width) / 2) && mouseX <= ((WINWIDTH - Image1Width) / 2) + Image1Width &&
                            mouseY >= ((WINHEIGHT - Image1Height) / 2) && mouseY <= ((WINHEIGHT - Image1Height) / 2) + Image1Height && showMenu == true) {
                    startGame = true;
                    retourMenu = false;
                    showMenu = true;
                } else if (mouseX >= (WINWIDTH - Image2Width) && mouseX <= WINWIDTH &&
                    mouseY >= 0 && mouseY <= Image2Height) {
                    showMenu = false;
                }
        } else if (e.type == SDL_KEYUP ) {
            if (e.key.keysym.sym == SDLK_ESCAPE && showMenu == false) {
                showMenu = true;
            }
        }
        
    }
    if (prevShowMenu != showMenu) {
        if (!showMenu) {
            toggleMusic();
        } else {
            toggleMusic();
        }
        prevShowMenu = showMenu;
    }

    drawMenu();
}

void interactionPauseJeu() {
    if (e.type == SDL_QUIT) {
        running = false;
    } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        
        if (afficherImage) {
            if (mouseX >= ((WINWIDTH - ImageQuitterJeuWidth) / 2) && mouseX <= ((WINWIDTH - ImageQuitterJeuWidth) / 2) + ImageQuitterJeuWidth &&
                mouseY >= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) && mouseY <= ((WINHEIGHT - ImageQuitterJeuHeight) / 2 + 50) + ImageQuitterJeuHeight) {
                quit = true;
                running = false;
            }
            else if (mouseX >= ((WINWIDTH - ImageRetourMenuWidth) / 2) && mouseX <= ((WINWIDTH - ImageRetourMenuWidth) / 2) + ImageRetourMenuWidth &&
                mouseY >= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) && mouseY <= ((WINHEIGHT - ImageRetourMenuHeight) / 2 - 50) + ImageRetourMenuHeight) {
                retourMenu = !retourMenu;
                toggleMusic();
                afficherImage = !afficherImage;
            }
            else if (mouseX >= (WINWIDTH - ImageParametrePauseWidth) && mouseX <= WINWIDTH && 
                mouseY >= 0 && mouseY <= ImageParametrePauseHeight) {
                parametre = !parametre;
                afficherImage = false;
            } else if (mouseX >= 0 && mouseX <= ImageRetourArrièreWidth &&
                mouseY >= 0 && mouseY <= ImageRetourArrièreHeight && afficherImage == true) {
                if (afficherImage == true) {
                    afficherImage = !afficherImage;
                    toggleMusic();
                }
            }   
        }
    } else if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_ESCAPE && afficherImage == false && parametre == false) {
            afficherImage = !afficherImage;
            toggleMusic();
            parametre = false;
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE && afficherImage == true) {
            afficherImage = !afficherImage;
            toggleMusic();
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE && parametre == true) {
            parametre = !parametre;
            afficherImage=!afficherImage;
        }
    }
}
