#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

/* fonction qui affiche une image qui prend en paramètre le renderer, la position x et y, le scale (pour preserver les proportions initiales) et le chemin de l'image */

void render_sprite(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int scale, const char *image_path){
    if (!texture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
        exit(-1);
    }
    Uint32 format;
    int access, width, height;
    SDL_QueryTexture(texture, &format, &access, &width, &height);
    SDL_Rect dest_rect = {x, y, scale*width, scale*height};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
}


/* fonction qui sert à afficher du texte */
void print_string(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int scale){
    if (!texture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in font texture init: %s", IMG_GetError());
        exit(-1);
    }
    int textWidth, textHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect dest_rect = {x, y, textWidth * scale, textHeight * scale};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
}


char *strdup(const char *c)
{
    char *dup = malloc(strlen(c) + 1);

    if (dup != NULL)
       strcpy(dup, c);

    return dup;
}

void text_in_box(const char *text, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer, int x, int y, int scale, int maxWidth) {

    int currentX = x;
    int currentY = y;

    char *text_copy = strdup(text);
    char *deb = text_copy;
    
    char *word = strtok((char *)text_copy, " ");
    while (word != NULL) {
        SDL_Surface *surface = TTF_RenderText_Blended(font, word, color);
        if (surface == NULL) {
            puts("Error in surface init");
            exit(-1);
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (texture == NULL) {
            puts("Error in texture init");
            exit(-1);
        }

        int textWidth, textHeight;
        SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);

        if (currentX + textWidth * scale > maxWidth) {
            currentX = x;
            currentY += textHeight * scale ;
        }
        SDL_Rect dstRect = {currentX, currentY, textWidth * scale, textHeight * scale};
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        currentX += textWidth * scale + 30;
        SDL_DestroyTexture(texture);
        word = strtok(NULL, " ");
    }
    free(deb);
    free(word);
}



