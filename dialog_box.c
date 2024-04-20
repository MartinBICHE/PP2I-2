#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
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
int first_word_length(const char *str) {
    int length = 0;
    while (*str != '\0' && *str != ' ') {
        length++;
        str++;
    }
    return length;
}

char *remove_first_word(const char *str) {
    int word_length = 0;
    while (*str != '\0' && *str != ' ') {
        word_length++;
        str++;
    }

    char *result = (char *)malloc(strlen(str) - word_length + 1);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strcpy(result, str + word_length);

    return result;
}


char* getFirstWord(const char *str) {
    int len = strlen(str);
    
    // Allocate memory for the first word
    char *first_word = (char*)malloc(len + 1);
    if (first_word == NULL) {
        return NULL; // Memory allocation failed
    }
    
    int i;
    int word_len = 0;
    
    // Find the length of the first word
    for (i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            first_word[word_len++] = str[i];
        } else {
            break;
        }
    }
    
    // Null-terminate the first word string
    first_word[word_len] = '\0';
    
    return first_word;
}

char* replaceFirstWordWithSpaces(const char *str) {
    int i;
    int len = strlen(str);
    
    int word_len = 0;
    for (i = 0; i < len; i++) {
        if (isspace(str[i])) {
            break;
        }
        word_len++;
    }
    
    char *result = (char*)malloc(len + 1);
    if (result == NULL) {
        return NULL; 
    }
    strcpy(result, str);
    
    for (i = 0; i < word_len; i++) {
        result[i] = ' ';
    }
    
    return result;
}
/* à mettre plus d'espace au début */

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color, SDL_Rect *dst_rect, SDL_Texture *boxTexture, TTF_Font *fontBold){
    static int currentCharIndex = 0;
    int textLength = strlen(text);
    int delay = 90;
    static Uint32 delayTimer = 0;
    const int pad = 20;
    int lenName = first_word_length(text);
    /* const char *textWithoutName = remove_first_word(text); */
    static int counter = 0;

    SDL_Surface *surfaceText = TTF_RenderText_Blended_Wrapped(font, text, color, 200);
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    Uint32 formatInit;
    int accessInit, textWidthInit, textHeightInit;
    SDL_QueryTexture(textureText, &formatInit, &accessInit, &textWidthInit, &textHeightInit);
    SDL_Rect dstRectBox = {dst_rect->x - pad, dst_rect->y - pad, textWidthInit + 2*pad, textHeightInit + 2*pad};
    SDL_RenderCopy(renderer, boxTexture, NULL, &dstRectBox);
    SDL_FreeSurface(surfaceText);
    SDL_DestroyTexture(textureText);
    /* SDL_Surface *surface; */
    /* const char *textWithSpaces = replaceFirstWordWithSpaces(text); */

    if (currentCharIndex < textLength){
        char currentText[currentCharIndex + 2];
        /* strncpy(currentText, text, currentCharIndex + 1); */
        /* currentText[currentCharIndex + 1] = '\0'; */
        TTF_Font *currentFont;
        if (counter <= lenName) {
            currentFont = fontBold;
            strncpy(currentText, text, currentCharIndex + 1);
            currentText[currentCharIndex + 1] = '\0';
        } else {
            char *textWithSpaces = replaceFirstWordWithSpaces(text);
            strncpy(currentText, textWithSpaces, currentCharIndex + 1);
            free(textWithSpaces);
            currentText[currentCharIndex + 1] = '\0';
            currentFont = font;
            char *firstWord = getFirstWord(text);
            SDL_Surface *surfaceName = TTF_RenderText_Blended_Wrapped(fontBold, firstWord, color, 200);
            free(firstWord);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceName);
            Uint32 format;
            int access, textWidth, textHeight;
            SDL_QueryTexture(texture, &format, &access, &textWidth, &textHeight);
            dst_rect->h = textHeight;
            dst_rect->w = textWidth;
            SDL_RenderCopy(renderer, texture, NULL, dst_rect);
            }
            SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(currentFont, currentText, color, 200);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        Uint32 format;
        int access, textWidth, textHeight;
        SDL_QueryTexture(texture, &format, &access, &textWidth, &textHeight);
        dst_rect->h = textHeight;
        dst_rect->w = textWidth;
        SDL_RenderCopy(renderer, texture, NULL, dst_rect);
        SDL_DestroyTexture(texture);
        if (SDL_GetTicks() - delayTimer >= delay){
            currentCharIndex ++;
            delayTimer = SDL_GetTicks();
            counter ++;
        }

    }else{
        char *textWithSpaces = replaceFirstWordWithSpaces(text);
        SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, textWithSpaces, color, 200);
        free(textWithSpaces);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        char *firstWord = getFirstWord(text);
        SDL_Surface *surfaceFirstWord = TTF_RenderText_Blended(fontBold, firstWord, color);
        free(firstWord);
        SDL_Texture *textureFirstWord = SDL_CreateTextureFromSurface(renderer, surfaceFirstWord);
        SDL_FreeSurface(surfaceFirstWord);
        SDL_FreeSurface(surface);
        Uint32 format;
        int access, textWidth, textHeight;
        SDL_QueryTexture(texture, &format, &access, &textWidth, &textHeight);
        SDL_Rect dst_rectFirstWord;
        Uint32 formatFirstWord;
        int accessFirstWord, textWidthFirstWord, textHeightFirstWord;
        SDL_QueryTexture(textureFirstWord, &formatFirstWord, &accessFirstWord, &textWidthFirstWord, &textHeightFirstWord);
        dst_rectFirstWord.x = dst_rect->x;
        dst_rectFirstWord.y = dst_rect->y;
        dst_rect->h = textHeight;
        dst_rect->w = textWidth;
        dst_rectFirstWord.h = textHeightFirstWord;
        dst_rectFirstWord.w = textWidthFirstWord;
        SDL_RenderCopy(renderer, texture, NULL, dst_rect);
        SDL_RenderCopy(renderer, textureFirstWord, NULL, &dst_rectFirstWord);
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(textureFirstWord);
    }

}
