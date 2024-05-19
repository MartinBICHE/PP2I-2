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
    res->hitbox = (SDL_Rect){.x = (res->x - PERSO_WIDTH/2.0f)*map->pix_rect, .y = (res->y - PERSO_HEIGHT/2.0f)*map->pix_rect, .w = PERSO_WIDTH*map->pix_rect, .h = PERSO_HEIGHT*map->pix_rect};
    res->jumps = 2;
    res->jump_delay = 25;
    res->health = 9;
    return res;
}


int display_perso(SDL_Renderer *renderer, Perso *perso, Map *map) {
    SDL_Rect rect1 = {.x = perso->hitbox.x - map->x_cam, .y = perso->hitbox.y, .w = perso->hitbox.w, .h = perso->hitbox.h};
    if (SDL_RenderDrawRect(renderer, &rect1)) {
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
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
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
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
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
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i-1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i-1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i+1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i+1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
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
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i-1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i-1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i+1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i+1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
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
    perso->vy += currentGravity*DT;
    perso->jump_delay = max(perso->jump_delay - 1,0);
    int i = floor(perso->y);
    int j = floor(perso->x);
    if (currentGravity > 0) {
        if (hitbox_bottom(perso, map)) {
            perso->vy = min(perso->vy, 0.0f);
            perso->y = i + 1 - PERSO_HEIGHT/2.0f;
            perso->jumps = 1;
        }
        if (hitbox_top(perso, map)) {
            perso->vy = max(perso->vy, 0.0f);
            perso->y = i + PERSO_HEIGHT/2.0f;
        }
    } else {
        if (hitbox_bottom(perso, map)) {
            perso->vy = min(perso->vy, 0.0f);
            perso->y = i + 1 - PERSO_HEIGHT/2.0f;
        }
        if (hitbox_top(perso, map)) {
            perso->vy = max(perso->vy, 0.0f);
            perso->y = i + PERSO_HEIGHT/2.0f;
            perso->jumps = 1;
        }
    }
    if (hitbox_left(perso, map)) {
        perso->vx = max(perso->vx, 0.0f);
        perso->x = j + PERSO_WIDTH/2.0f;
    }
    if (hitbox_right(perso, map)) {
        perso->vx = min(perso->vx, 0.0f);
        perso->x = j+1 - PERSO_WIDTH/2.0f;
    }
    perso->y += perso->vy*DT;
    perso->x += perso->vx*DT;
    perso->hitbox = (SDL_Rect){.x = (perso->x - PERSO_WIDTH/2.0f)*map->pix_rect, .y = (perso->y - PERSO_HEIGHT/2.0f)*map->pix_rect, .w = PERSO_WIDTH*map->pix_rect, .h = PERSO_HEIGHT*map->pix_rect};
}


void jump(Perso *perso, Map *map) {
    if (perso->jumps > 0 && perso->jump_delay == 0) {
        if (currentGravity < 0) {
            perso->vy = -JUMPSPEED_INVERTED;
        } else {
            perso->vy = -JUMPSPEED;
        }
        perso->jumps -= 1;
        perso->jump_delay = 25;
    }
}

void changeGravity(void) {
	if (currentGravity == ACC) {
		currentGravity = ACC_INVERTED;
		jumpSpeed = JUMPSPEED_INVERTED;
	} else {
		currentGravity = ACC;
		jumpSpeed = JUMPSPEED;
	}
}
