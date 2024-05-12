#include "boss.h"
#include <stdlib.h>
#include "perso.h"

// Créer un nouveau boss
Boss *create_boss(Map* map) {
    Boss* boss = malloc(sizeof(Boss));
    // Position de départ du boss sur la carte
    boss->x = map->start_xboss; 
    boss->y = map->start_yboss; 
    boss->vx = BOSS_SPEED;
    boss->hitbox = (SDL_Rect){.x = (boss->x - BOSS_WIDTH/2.0f) * map->pix_rect, .y = (boss->y - BOSS_HEIGHT/2.0f) * map->pix_rect, .w = BOSS_WIDTH * map->pix_rect, .h = BOSS_HEIGHT * map->pix_rect};
    return boss;
}

int displayBoss(SDL_Renderer *renderer, Boss *boss, Map *map) {
    SDL_Rect rect = {.x = boss->hitbox.x - map->x_cam, .y = boss->hitbox.y, .w = boss->hitbox.w, .h = boss->hitbox.h};
    if (SDL_RenderDrawRect(renderer, &rect)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in draw rect : %s", SDL_GetError());
		exit(-1);
    }
    return 0;
}


int hitboxBottomBoss(Boss *boss, Map *map) {
    SDL_Rect hbb = {.x = boss->hitbox.x + 2, .y = boss->hitbox.y + boss->hitbox.h + 3, .w = boss->hitbox.w - 4, .h = 1};
    SDL_Rect res;

    int i = boss->y + 1;
    int j = boss->x;
    if (i >= map->height || j >= map->width || i < 0 || j < 0)
        return 0;

    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res))
            return 1;
    }
    if (j - 1 >= 0 && map->matrix[i][j - 1] != '-') {
        SDL_Rect rect = {.x = (j - 1) * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res))
            return 1;
    }
    if (j + 1 < map->width && map->matrix[i][j + 1] != '-') {
        SDL_Rect rect = {.x = (j + 1) * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res))
            return 1;
    }
    return 0;
}

int hitboxTopBoss(Boss *boss, Map *map) {
    SDL_Rect hbt = {.x = boss->hitbox.x + 1, .y = boss->hitbox.y - 3, .w = boss->hitbox.w - 2, .h = 1};
    SDL_Rect res;

    int i = boss->y - 1;
    int j = boss->x;
    if (i < 0 || j < 0 || i >= map->height || j >= map->width)
        return 0;

    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res))
            return 1;
    }
    if (j - 1 >= 0 && map->matrix[i][j - 1] != '-') {
        SDL_Rect rect = {.x = (j - 1) * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res))
            return 1;
    }
    if (j + 1 < map->width && map->matrix[i][j + 1] != '-') {
        SDL_Rect rect = {.x = (j + 1) * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res))
            return 1;
    }
    return 0;
}

int hitboxLeftBoss(Boss *boss, Map *map) {
    SDL_Rect hbl = {.x = boss->hitbox.x - 3, .y = boss->hitbox.y + 7, .w = 1, .h = boss->hitbox.h - 12};
    SDL_Rect res;

    int i = boss->y;
    int j = boss->x - 1;
    if (i < 0 || j < 0 || i >= map->height || j >= map->width)
        return 0;

    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res))
            return 1;
    }
    if (i - 1 >= 0 && map->matrix[i - 1][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = (i - 1) * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res))
            return 1;
    }
    if (i + 1 < map->height && map->matrix[i + 1][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = (i + 1) * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res))
            return 1;
    }
    return 0;
}

int hitboxRightBoss(Boss *boss, Map *map) {
    SDL_Rect hbr = {.x = boss->hitbox.x + boss->hitbox.w + 3, .y = boss->hitbox.y + 7, .w = 1, .h = boss->hitbox.h - 12};
    SDL_Rect res;

    int i = boss->y;
    int j = boss->x + 1;
    if (i < 0 || j < 0 || i >= map->height || j >= map->width)
        return 0;

    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = i * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res))
            return 1;
    }
    if (i - 1 >= 0 && map->matrix[i - 1][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = (i - 1) * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res))
            return 1;
    }
    if (i + 1 < map->height && map->matrix[i + 1][j] != '-') {
        SDL_Rect rect = {.x = j * map->pix_rect, .y = (i + 1) * map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res))
            return 1;
    }
    return 0;
}


void updateBoss(Boss *boss, Map *map) {
    boss->vy += currentGravity * DT;
    int i = floor(boss->y);
    int j = floor(boss->x);

    if (boss->x < 1.4 || boss->x > map->width - BOSS_WIDTH - 0.7) {
        boss->vx = -boss->vx; // Inverser la direction horizontale
    }

    static bool gravityChanged = false;

    if (currentGravity > 0) {
        if (hitboxBottomBoss(boss, map)) {
            boss->vy = min(boss->vy, 0.0f);
            boss->y = i + 1 - BOSS_HEIGHT / 2.0f;
        }
        if (hitboxTopBoss(boss, map)) {
            boss->vy = max(boss->vy, 0.0f);
            boss->y = i + BOSS_HEIGHT / 2.0f;
        }
    } else {
        if (hitboxBottomBoss(boss, map)) {
            boss->vy = min(boss->vy, 0.0f);
            boss->y = i + 1 - BOSS_HEIGHT / 2.0f;
        }
        if (hitboxTopBoss(boss, map)) {
            boss->vy = max(boss->vy, 0.0f);
            boss->y = i + BOSS_HEIGHT / 2.0f;
        }
    }

    if (boss->x > 1.5 && boss->x < map->width - BOSS_WIDTH - 0.8) {
        if (!gravityChanged) {
            if (hitboxLeftBoss(boss, map)) {
                boss->vx = max(boss->vx, 0.0f);
                boss->x = j + BOSS_WIDTH / 2.0f;
            }
            if (hitboxRightBoss(boss, map)) {
                boss->vx = min(boss->vx, 0.0f);
                boss->x = j + 1 - BOSS_WIDTH / 2.0f;
            }
        }
    }

    gravityChanged = (currentGravity != 0);

    boss->y += boss->vy * DT;
    boss->x += boss->vx * DT;
    boss->hitbox = (SDL_Rect){
        .x = (boss->x - BOSS_WIDTH / 2.0f) * map->pix_rect,
        .y = (boss->y - BOSS_HEIGHT / 2.0f) * map->pix_rect,
        .w = BOSS_WIDTH * map->pix_rect,
        .h = BOSS_HEIGHT * map->pix_rect};
}
