#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "dialog_box.h"


/* Exemple d'utilisation, à utiliser impérativement avec TTF_RenderText_Blended_Wrapped  et à mettre pour le texte "            puis début du texte"*/
/* SDL_Texture *textureBox = SDL_CreateTextureFromSurface(renderer, surfaceBox); */
/* SDL_Texture *textureBoxName = SDL_CreateTextureFromSurface(renderer, surfaceBoxName); */
/* SDL_Surface *surfaceBox = IMG_Load("papirus.png"); */
/* SDL_Surface *surfaceBoxName = IMG_Load("transparent.png"); */
/* SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font1, text, BLACK, 200); */
/* SDL_Surface *textSurfaceName = TTF_RenderText_Blended_Wrapped(font2, textName, BLACK, 200); */
/* SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); */
/* SDL_Texture *textTextureName = SDL_CreateTextureFromSurface(renderer, textSurfaceName); */
/* render_box(renderer, textureBox, textTexture, 10, 10); */
/* render_box(renderer, textureBoxName, textTextureName, 10, 10); */
/* TTF_Font *font1 = TTF_OpenFont("DisposableDroidBB.ttf", 20); */
/* TTF_Font *font2 = TTF_OpenFont("DisposableDroidBB_bld.ttf", 20); */


void render_box(SDL_Renderer *renderer, SDL_Texture *boxTexture, SDL_Texture *textTexture, int x, int y){
    const int pad = 20;
    if (!boxTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }
    if (!textTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }
    Uint32 format;
    int access, textWidth, textHeight;
    SDL_QueryTexture(textTexture, &format, &access, &textWidth, &textHeight);
    SDL_Rect dstRectBox = {x, y, textWidth + 2*pad, textHeight + 2*pad};
    SDL_Rect dstRectText = {x + pad, y + pad , textWidth, textHeight};
    SDL_RenderCopy(renderer, boxTexture, NULL, &dstRectBox);
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRectText);




}
