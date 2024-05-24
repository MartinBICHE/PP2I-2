#include "boss.h"
#include <stdlib.h>
#include "perso.h"
#include "projectile.h"

// Définition des parcours
const int parcours[1][20] = {
    {0,2,0,2,2,0,0,0,1,3,1,1,3,2,1,3,3,2,1,3}
};
// 0 : haut
// 1 : bas 
// 2 : gauche
// 3 : droite
// Parcours 1 : {0,2,0,2,2,2}

// Créer un nouveau boss
Boss *create_boss(Map* map) {
    Boss* boss = malloc(sizeof(Boss));
    boss->x = map->start_xboss; 
    boss->y = map->start_yboss; 
    boss->vx = 0;
    boss->vy = 0;
    boss->hitbox = (SDL_Rect){.x = (boss->x - BOSS_WIDTH/2.0f) * map->pix_rect, .y = (boss->y - BOSS_HEIGHT/2.0f) * map->pix_rect, .w = BOSS_WIDTH * map->pix_rect, .h = BOSS_HEIGHT * map->pix_rect};
    boss->lastMoveTime = SDL_GetTicks(); // Initialiser le temps du dernier mouvement
    boss->parcoursIndex = rand() % 1; // Choisir un parcours aléatoire parmi les deux définis
    boss->currentMove = 0;
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
    // Récupérer la direction du parcours actuel
    boss->direction = parcours[boss->parcoursIndex][boss->currentMove];

    float new_x = boss->x;
    float new_y = boss->y;

    switch (boss->direction) {
        case 0: // Haut
            new_y -= 1;
            break;
        case 1: // Basg
            new_y += 1;
            break;
        case 2: // Gauche
            new_x -= 1;
            break;
        case 3: // Droite
            new_x += 1;
            break;
    }

    // Vérifier les obstacles
    boss->x = new_x;
    boss->y = new_y;

    boss->hitbox.x = (boss->x - BOSS_WIDTH / 2.0f) * map->pix_rect;
    boss->hitbox.y = (boss->y - BOSS_HEIGHT / 2.0f) * map->pix_rect;

    // Passer au mouvement suivant du parcours
    boss->currentMove = (boss->currentMove + 1) % 20; // Parcours de 5 déplacements
}
