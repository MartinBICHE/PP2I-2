#ifndef scroll_h 
#define scroll_h
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "const.h"
#include <SDL2/SDL_ttf.h>

enum ScrollState {
  SCROLL_MOVING_DOWN,
  SCROLL_MOVING_UP,
  SCROLL_PAUSE_BOTTOM,
  SCROLL_SECOND_PAUSE,
};

struct _ScrollStateData{
    enum ScrollState state;
    Uint32 pauseStart;
    int currentCharIndex;
    Uint32 delayTimer;
    SDL_Rect dst_rect;
    SDL_Rect src_rect;
};

typedef struct _ScrollStateData ScrollStateData;


void initScroll(ScrollStateData *scrollStateData, int x, int y);

void scroll_movement(SDL_Renderer *renderer,
                       const char *text, SDL_Color color,
                       ScrollStateData *scrollStateData, float x_cam);
#endif
