#include "dialog_box.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "fonts.h"
#include "textures.h"

/* s'utilise avec: */
/* DialogBoxData dialogBoxData; */
/* initPapirus(&dialogBoxData, xPos(à définir), yPos(à définir)); */
/* const char *text = text(à définir); */
/* render_text(renderer, text, BLACK, &dialogBoxData); */

int first_word_length(const char *str) {
  int length = 0;
  while (*str != '\0' && *str != ' ') {
    length++;
    str++;
  }
  return length;
}

char *getFirstWord(const char *str) {
  int len = strlen(str);

  char *first_word = (char *)malloc(len + 1);
  if (first_word == NULL) {
    return NULL;
  }

  int i;
  int word_len = 0;

  for (i = 0; i < len; i++) {
    if (!isspace(str[i])) {
      first_word[word_len++] = str[i];
    } else {
      break;
    }
  }

  first_word[word_len] = '\0';

  return first_word;
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

char *replaceFirstWordWithSpaces(const char *str) {
  int i;
  int len = strlen(str);

  int word_len = 0;
  for (i = 0; i < len; i++) {
    if (isspace(str[i])) {
      break;
    }
    word_len++;
  }

  char *result = (char *)malloc(len + 1);
  if (result == NULL) {
    return NULL;
  }
  strcpy(result, str);

  for (i = 0; i < word_len; i++) {
    result[i] = ' ';
  }

  return result;
}

void render_text(SDL_Renderer *renderer, const char *text,
                 SDL_Color color, DialogBoxData *dialogBoxData, float x_cam){
  int textLength = strlen(text);
  int delay = 90;
  const int pad = 20;
  int lenName = first_word_length(text);

  SDL_Surface *surfaceText =
      TTF_RenderText_Blended_Wrapped(fontDialogBox, text, color, 200);
  SDL_Texture *textureText =
      SDL_CreateTextureFromSurface(renderer, surfaceText);
  Uint32 formatInit;
  int accessInit, textWidthInit, textHeightInit;
  SDL_QueryTexture(textureText, &formatInit, &accessInit, &textWidthInit,
                   &textHeightInit);
  SDL_Rect dstRectBox = {dialogBoxData->dst_rect.x - pad,
                         dialogBoxData->dst_rect.y - pad,
                         textWidthInit + 2 * pad, textHeightInit + 2 * pad};
  SDL_Rect dstRectDisplay = {dstRectBox.x - x_cam, dstRectBox.y, dstRectBox.w, dstRectBox.h};
  /* SDL_RenderCopy(renderer, texturePapirus, NULL, &dstRectBox); */
  SDL_RenderCopy(renderer, texturePapirus, NULL, &dstRectDisplay);
  SDL_FreeSurface(surfaceText);
  SDL_DestroyTexture(textureText);

  if (dialogBoxData->currentCharIndex < textLength) {
    char currentText[dialogBoxData->currentCharIndex + 2];
    TTF_Font *currentFont;
    if (dialogBoxData->counter <= lenName) {
      currentFont = fontDialogBoxBold;
      strncpy(currentText, text, dialogBoxData->currentCharIndex + 1);
      currentText[dialogBoxData->currentCharIndex + 1] = '\0';
    } else {
      char *textWithSpaces = replaceFirstWordWithSpaces(text);
      strncpy(currentText, textWithSpaces, dialogBoxData->currentCharIndex + 1);
      free(textWithSpaces);
      currentText[dialogBoxData->currentCharIndex + 1] = '\0';
      currentFont = fontDialogBox;
      char *firstWord = getFirstWord(text);
      SDL_Surface *surfaceName =
          TTF_RenderText_Blended_Wrapped(fontDialogBoxBold, firstWord, color, 200);
      free(firstWord);
      SDL_Texture *texture =
          SDL_CreateTextureFromSurface(renderer, surfaceName);
      Uint32 format;
      int access, textWidth, textHeight;
      SDL_QueryTexture(texture, &format, &access, &textWidth, &textHeight);
      dialogBoxData->dst_rect.h = textHeight;
      dialogBoxData->dst_rect.w = textWidth;
      SDL_Rect dst_rectFixed = {dialogBoxData->dst_rect.x - x_cam, dialogBoxData->dst_rect.y, dialogBoxData->dst_rect.w, dialogBoxData->dst_rect.h};
      SDL_RenderCopy(renderer, texture, NULL, &dst_rectFixed);
      /* SDL_RenderCopy(renderer, texture, NULL, &dialogBoxData->dst_rect); */
      SDL_FreeSurface(surfaceName);
      SDL_DestroyTexture(texture);
    }
    SDL_Surface *surface =
        TTF_RenderText_Blended_Wrapped(currentFont, currentText, color, 200);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    Uint32 format;
    int access, textWidth, textHeight;
    SDL_QueryTexture(texture, &format, &access, &textWidth, &textHeight);
    dialogBoxData->dst_rect.h = textHeight;
    dialogBoxData->dst_rect.w = textWidth;
    SDL_Rect dst_rectFixed = {dialogBoxData->dst_rect.x - x_cam, dialogBoxData->dst_rect.y, dialogBoxData->dst_rect.w, dialogBoxData->dst_rect.h};
    SDL_RenderCopy(renderer, texture, NULL, &dst_rectFixed);
    /* SDL_RenderCopy(renderer, texture, NULL, &dialogBoxData->dst_rect); */
    SDL_DestroyTexture(texture);
    if (SDL_GetTicks() - dialogBoxData->delayTimer >= delay) {
      dialogBoxData->currentCharIndex++;
      dialogBoxData->delayTimer = SDL_GetTicks();
      dialogBoxData->counter++;
    }

  } else {
    char *textWithSpaces = replaceFirstWordWithSpaces(text);
    SDL_Surface *surface =
        TTF_RenderText_Blended_Wrapped(fontDialogBox, textWithSpaces, color, 200);
    free(textWithSpaces);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    char *firstWord = getFirstWord(text);
    SDL_Surface *surfaceFirstWord =
        TTF_RenderText_Blended(fontDialogBoxBold, firstWord, color);
    free(firstWord);
    SDL_Texture *textureFirstWord =
        SDL_CreateTextureFromSurface(renderer, surfaceFirstWord);
    SDL_FreeSurface(surfaceFirstWord);
    SDL_FreeSurface(surface);
    Uint32 format;
    int access, textWidth, textHeight;
    SDL_QueryTexture(texture, &format, &access, &textWidth, &textHeight);
    SDL_Rect dst_rectFirstWord;
    Uint32 formatFirstWord;
    int accessFirstWord, textWidthFirstWord, textHeightFirstWord;
    SDL_QueryTexture(textureFirstWord, &formatFirstWord, &accessFirstWord,
                     &textWidthFirstWord, &textHeightFirstWord);
    dst_rectFirstWord.x = dialogBoxData->dst_rect.x;
    dst_rectFirstWord.y = dialogBoxData->dst_rect.y;
    dialogBoxData->dst_rect.h = textHeight;
    dialogBoxData->dst_rect.w = textWidth;
    dst_rectFirstWord.h = textHeightFirstWord;
    dst_rectFirstWord.w = textWidthFirstWord;
    SDL_Rect dst_rectFixed = {dialogBoxData->dst_rect.x - x_cam, dialogBoxData->dst_rect.y, dialogBoxData->dst_rect.w, dialogBoxData->dst_rect.h};
    SDL_RenderCopy(renderer, texture, NULL, &dst_rectFixed);
    /* SDL_RenderCopy(renderer, texture, NULL, &dialogBoxData->dst_rect); */
    /* SDL_RenderCopy(renderer, texture, NULL, &dst_rectFirstWord); */
    SDL_Rect dst_rectFixedName = {dst_rectFirstWord.x - x_cam, dst_rectFirstWord.y, dst_rectFirstWord.w, dst_rectFirstWord.h};
    SDL_RenderCopy(renderer, textureFirstWord, NULL, &dst_rectFixedName);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(textureFirstWord);
  }
}

void initPapirus(DialogBoxData *dialogBoxData, int x, int y) {

  dialogBoxData->dst_rect.x = x;
  dialogBoxData->dst_rect.y = y;
  dialogBoxData->dst_rect.w = 64;
  dialogBoxData->dst_rect.h = 64;
  dialogBoxData->currentCharIndex = 0;
  dialogBoxData->delayTimer = 0;
  dialogBoxData->counter = 0;
}

