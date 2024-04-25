#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "const.h"
#include "perso.h"


Perso *create_perso(Map *map) {
    Perso *res = malloc(sizeof(Perso));
    res->x = map->start_x;
    res->y = map->start_y;
    res->vx = 0;
    res->vy = 0;
    res->hitbox = (SDL_Rect){.x = res->x*PIX_RECT - PERSO_WIDTH/2, .y = res->y*PIX_RECT - PERSO_HEIGHT/2, .w = PERSO_WIDTH, .h = PERSO_HEIGHT};
    return res;
}


int display_perso(SDL_Renderer *renderer, Perso *perso, float x_cam) {
    SDL_Rect rect1 = {.x = perso->x*PIX_RECT - 30 - x_cam, .y = perso->y*PIX_RECT - 30, .w = 60, .h = 60};
    if (SDL_RenderDrawRect(renderer, &rect1)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in draw rect : %s", SDL_GetError());
		exit(-1);
    }
    SDL_Rect rect2 = {.x = perso->x*PIX_RECT - 28 - x_cam, .y = perso->y*PIX_RECT - 28, .w = 56, .h = 56};
    if (SDL_RenderDrawRect(renderer, &rect2)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in draw rect : %s", SDL_GetError());
		exit(-1);
    }
    return 0;
}


int hitbox_bottom(Perso *perso, Map *map) {
    SDL_Rect hbb = {.x = perso->hitbox.x + 2, .y = perso->hitbox.y + perso->hitbox.h + 3, .w = perso->hitbox.w - 4, .h = 1};
    SDL_Rect res;
    int i = perso->y + 1;
    int j = perso->x;
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    return 0;
}


int hitbox_top(Perso *perso, Map *map) {
    SDL_Rect hbt = {.x = perso->hitbox.x + 1, .y = perso->hitbox.y - 3, .w = perso->hitbox.w - 2, .h = 1};
    SDL_Rect res;
    int i = perso->y - 1;
    int j = perso->x;
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    return 0;
}


int hitbox_left(Perso *perso, Map *map) {
    SDL_Rect hbl = {.x = perso->hitbox.x - 3, .y = perso->hitbox.y + 7, .w = 1, .h = perso->hitbox.h - 12};
    SDL_Rect res;
    int i = perso->y;
    int j = perso->x - 1;
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i-1][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = (i-1)*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i+1][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = (i+1)*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    return 0;
}


int hitbox_right(Perso *perso, Map *map) {
    SDL_Rect hbr = {.x = perso->hitbox.x + perso->hitbox.w + 3, .y = perso->hitbox.y + 7, .w = 1, .h = perso->hitbox.h - 12};
    SDL_Rect res;
    int i = perso->y;
    int j = perso->x + 1;
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = i*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i-1][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = (i-1)*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i+1][j] != '-') {
        SDL_Rect rect = {.x = j*PIX_RECT, .y = (i+1)*PIX_RECT, .w = PIX_RECT, .h = PIX_RECT};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    return 0;
}


float max(float a, float b) {
    if (a<b)return b;
    else return a;
}


float min(float a, float b) {
    if (a>b)return b;
    else return a;
}


void updatePerso(Perso *perso, Map *map) {
    perso->hitbox = (SDL_Rect){.x = perso->x*PIX_RECT - PERSO_WIDTH/2, .y = perso->y*PIX_RECT - PERSO_HEIGHT/2, .w = PERSO_WIDTH, .h = PERSO_HEIGHT};
    int i = floor(perso->y);
    int j = floor(perso->x);
    perso->vy += ACC*DT;
    if (hitbox_bottom(perso, map)) {
        perso->vy = min(perso->vy, 0.0f);
        perso->y = i+1 - 0.32f;
    }
    if (hitbox_top(perso, map)) {
        perso->vy = max(perso->vy, 0.0f);
        perso->y = i + 0.33f;
    }
    if (hitbox_left(perso, map)) {
        perso->vx = max(perso->vx, 0.0f);
        perso->x = j + 0.33f;
    }
    if (hitbox_right(perso, map)) {
        perso->vx = min(perso->vx, 0.0f);
        perso->x = j+1 - 0.32f;
    }
    perso->y += perso->vy*DT;
    perso->x += perso->vx*DT;
}


void jump(Perso *perso, Map *map) {
    if (hitbox_bottom(perso, map)) {
        perso->vy = -JUMPSPEED;
    }
}


void destroy_perso(Perso *perso) {
    free(perso);
}
