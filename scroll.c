#include "scroll.h"
#include "SDL2/SDL_ttf.h"
#include "const.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include "textures.h"
#include "fonts.h"


/* s'utilise avec: */ 
/* ScrollStateData scrollStateData; */
/*   initScroll(&scrollStateData, xPos(à définir), yPos(à définir)); */
/* const char *text = "texte(à définir)"; */
/* scroll_movement(renderer, text, BLACK, &scrollStateData); */

void scroll_movement(SDL_Renderer *renderer, 
                       const char *text, SDL_Color color,
                       ScrollStateData *scrollStateData) {
  int speed = 1;
  int interval = 6000;
  int textLength = strlen(text);
  int delay = 90;
  const int pad = 40;
  SDL_Surface *surfaceText =
      TTF_RenderText_Blended_Wrapped(fontScroll, text, color, scrollStateData->dst_rect.w - 50);
  SDL_Texture *textureText =
      SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  Uint32 format;
  int access, textWidth, textHeight;
  SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
  SDL_RenderCopy(renderer, textureScroll, &scrollStateData->src_rect, &scrollStateData->dst_rect);
  SDL_Rect dst_rectText = {scrollStateData->dst_rect.x + pad, scrollStateData->dst_rect.y + pad, 5, 5};
  int padText = 100;

  switch (scrollStateData->state) {
  case SCROLL_MOVING_DOWN:
    if (scrollStateData->dst_rect.h <= textHeight + padText) {
      scrollStateData->dst_rect.h += speed;
      scrollStateData->src_rect.h += speed;
    } else {
      scrollStateData->state = SCROLL_PAUSE_BOTTOM;
      scrollStateData->pauseStart = SDL_GetTicks();
    }
    break;
  case SCROLL_PAUSE_BOTTOM:
    if (scrollStateData->currentCharIndex <= textLength) {
      char currentText[scrollStateData->currentCharIndex + 2];
      strncpy(currentText, text, scrollStateData->currentCharIndex + 1);
      currentText[scrollStateData->currentCharIndex + 1] = '\0';
      SDL_Surface *surfaceText = TTF_RenderText_Blended_Wrapped(
          fontScroll, currentText, color, scrollStateData->dst_rect.w - 50);
      SDL_Texture *textureText =
          SDL_CreateTextureFromSurface(renderer, surfaceText);
      SDL_FreeSurface(surfaceText);
      Uint32 format;
      int access, textWidth, textHeight;
      SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
      dst_rectText.h = textHeight;
      dst_rectText.w = textWidth;
      SDL_RenderCopy(renderer, textureText, NULL, &dst_rectText);
      SDL_DestroyTexture(textureText);
      if (SDL_GetTicks() - scrollStateData->delayTimer >= delay) {
        scrollStateData->currentCharIndex++;
        scrollStateData->delayTimer = SDL_GetTicks();
      }
      /* SDL_DestroyTexture(textureText); */
    } else {
      scrollStateData->state = SCROLL_SECOND_PAUSE;
      scrollStateData->pauseStart = SDL_GetTicks();
    }
    break;
  case SCROLL_SECOND_PAUSE:
    if (SDL_GetTicks() - scrollStateData->pauseStart >= interval) {
      scrollStateData->state = SCROLL_MOVING_UP;
    }
    SDL_Surface *surfaceText =
        TTF_RenderText_Blended_Wrapped(fontScroll, text, color, scrollStateData->dst_rect.w - 50);
    SDL_Texture *textureText =
        SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    Uint32 format;
    int access, textWidth, textHeight;
    SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
    dst_rectText.h = textHeight;
    dst_rectText.w = textWidth;
    SDL_RenderCopy(renderer, textureText, NULL, &dst_rectText);

    if (SDL_GetTicks() - scrollStateData->pauseStart >= interval) {
      scrollStateData->state = SCROLL_MOVING_UP;
    }
    break;

  case SCROLL_MOVING_UP:
    if (scrollStateData->dst_rect.y >= 0) {
      scrollStateData->dst_rect.h -= speed;
      scrollStateData->src_rect.h += speed;
    }
  }
}

void initScroll(ScrollStateData *scrollStateData, int x, int y){
  scrollStateData->pauseStart = 0;
  scrollStateData->currentCharIndex = 0;
  scrollStateData->delayTimer = 0;
  scrollStateData->src_rect.x = 0;
  scrollStateData->src_rect.y = 0;
  scrollStateData->src_rect.w = 250;
  scrollStateData->src_rect.h = 10;

  scrollStateData->dst_rect.x = x;
  scrollStateData->dst_rect.y = y;
  scrollStateData->dst_rect.w = 250;
  scrollStateData->dst_rect.h = 10;
}

