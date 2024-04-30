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


/* s'utilise avec: */ 
/* ScrollStateData scrollStateData; */
/*   initScroll(&scrollStateData, xPos(à définir), yPos(à définir)); */


/*     scroll_movement_2(renderer, textureScroll, font3, text(à définir), BLACK, &scrollStateData); */

void scroll_movement(SDL_Renderer *renderer, SDL_Texture *texture,
                     SDL_Rect *src_rect, SDL_Rect *dst_rect, TTF_Font *font,
                     const char *text, SDL_Color color) {
  int speed = 1;
  int interval = 6000;
  static enum ScrollState state = SCROLL_MOVING_DOWN;
  static Uint32 pauseStart = 0;

  static int currentCharIndex = 0;
  int textLength = strlen(text);
  int delay = 90;
  static Uint32 delayTimer = 0;
  const int pad = 40;
  /* dst_rectText->x = dst_rect->x + pad; */
  /* dst_rectText->y = dst_rect->y + pad; */
  SDL_Surface *surfaceText =
      TTF_RenderText_Blended_Wrapped(font, text, color, dst_rect->w - 50);
  SDL_Texture *textureText =
      SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  Uint32 format;
  int access, textWidth, textHeight;
  SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
  SDL_RenderCopy(renderer, texture, src_rect, dst_rect);
  SDL_Rect dst_rectText = {dst_rect->x + pad, dst_rect->y + pad, 5, 5};
  int padText = 130;

  switch (state) {
  case SCROLL_MOVING_DOWN:
    if (dst_rect->h <= textHeight + padText) {
      dst_rect->h += speed;
      src_rect->h += speed;
    } else {
      state = SCROLL_PAUSE_BOTTOM;
      pauseStart = SDL_GetTicks();
    }
    break;
  case SCROLL_PAUSE_BOTTOM:
    if (currentCharIndex <= textLength) {
      char currentText[currentCharIndex + 2];
      strncpy(currentText, text, currentCharIndex + 1);
      currentText[currentCharIndex + 1] = '\0';
      SDL_Surface *surfaceText = TTF_RenderText_Blended_Wrapped(
          font, currentText, color, dst_rect->w - 50);
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
      if (SDL_GetTicks() - delayTimer >= delay) {
        currentCharIndex++;
        delayTimer = SDL_GetTicks();
      }
      /* SDL_DestroyTexture(textureText); */
    } else {
      state = SCROLL_SECOND_PAUSE;
      pauseStart = SDL_GetTicks();
    }
    break;
  case SCROLL_SECOND_PAUSE:
    SDL_Surface *surfaceText =
        TTF_RenderText_Blended_Wrapped(font, text, color, dst_rect->w - 50);
    SDL_Texture *textureText =
        SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    Uint32 format;
    int access, textWidth, textHeight;
    SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
    dst_rectText.h = textHeight;
    dst_rectText.w = textWidth;
    SDL_RenderCopy(renderer, textureText, NULL, &dst_rectText);

    if (SDL_GetTicks() - pauseStart >= interval) {
      state = SCROLL_MOVING_UP;
    }
    break;

  case SCROLL_MOVING_UP:
    if (dst_rect->y >= 0) {
      dst_rect->h -= speed;
      src_rect->h += speed;
    }
  }
}
void scroll_movement_2(SDL_Renderer *renderer, SDL_Texture *texture,
                       TTF_Font *font, const char *text, SDL_Color color,
                       ScrollStateData *scrollStateData) {
  int speed = 1;
  int interval = 6000;
  int textLength = strlen(text);
  int delay = 90;
  const int pad = 40;
  SDL_Surface *surfaceText =
      TTF_RenderText_Blended_Wrapped(font, text, color, scrollStateData->dst_rect.w - 50);
  SDL_Texture *textureText =
      SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  Uint32 format;
  int access, textWidth, textHeight;
  SDL_QueryTexture(textureText, &format, &access, &textWidth, &textHeight);
  SDL_RenderCopy(renderer, texture, &scrollStateData->src_rect, &scrollStateData->dst_rect);
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
          font, currentText, color, scrollStateData->dst_rect.w - 50);
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
    SDL_Surface *surfaceText =
        TTF_RenderText_Blended_Wrapped(font, text, color, scrollStateData->dst_rect.w - 50);
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

