#include "boss.h"
#include <stdlib.h>
#include "perso.h"
#include "projectile.h"

// Définition des parcours
const int parcours[8][20] = {
    {0,2,0,2,2,0,0,0,1,3,1,1,3,2,1,3,3,2,1,3},
    {1,3,1,3,3,3,2,2,0,0,2,1,1,3,0,2,0,2,1,0},
    {0,3,3,3,1,1,1,3,2,0,2,0,2,1,1,2,3,2,0,0},
    {0,3,3,3,3,3,1,1,2,2,2,1,2,2,0,0,1,0,0,1},
    {1,3,1,3,3,1,1,1,0,2,0,0,2,3,0,2,2,3,0,2},
    {0,2,0,2,2,2,3,3,1,1,3,0,0,2,1,3,1,3,2,1},
    {1,2,2,2,0,0,0,2,3,1,3,1,3,0,0,3,2,3,1,1},
    {1,2,2,2,2,2,0,0,3,3,3,0,3,3,1,1,0,1,1,0}
};
// 0 : haut
// 1 : bas 
// 2 : gauche
// 3 : droite
// Parcours 1 :  {0,2,0,2,2,0,0,0,1,3,1,1,3,2,1,3,3,2,1,3}
// Parcours 2 :  {1,3,1,3,3,3,2,2,0,0,2,1,1,3,0,2,0,2,1,0}
// Parcours 3 :  {0,3,3,3,1,1,1,3,2,0,2,0,2,1,1,2,3,2,0,0}
// Parcours 4 :  {0,3,3,3,3,3,1,1,2,2,2,1,2,2,0,0,1,0,0,1}
// Parcours 5 :  {1,3,1,3,3,1,1,1,0,2,0,0,2,3,0,2,2,3,0,2}
// Parcours 6 :  {0,2,0,2,2,2,3,3,1,1,3,0,0,2,1,3,1,3,2,1}
// Parcours 7 :  {1,2,2,2,0,0,0,2,3,1,3,1,3,0,0,3,2,3,1,1}
// Parcours 8 :  {1,2,2,2,2,2,0,0,3,3,3,0,3,3,1,1,0,1,1,0}

// Créer un nouveau boss
Boss *create_boss(Map* map) {
    Boss* boss = malloc(sizeof(Boss));
    boss->x = map->start_xboss; 
    boss->y = map->start_yboss; 
    boss->vx = 0;
    boss->vy = 0;
    boss->hitbox = (SDL_Rect){.x = (boss->x - BOSS_WIDTH/2.0f) * map->pix_rect, .y = (boss->y - BOSS_HEIGHT/2.0f) * map->pix_rect, .w = BOSS_WIDTH * map->pix_rect, .h = BOSS_HEIGHT * map->pix_rect};
    boss->lastMoveTime = SDL_GetTicks(); // Initialiser le temps du dernier mouvement
    boss->parcoursIndex = rand() % 8; // Choisir un parcours aléatoire parmi les deux définis
    boss->currentMove = 0;
    boss->health = 9;
    boss->lastDamageTime = 0; // Initialiser le temps du dernier dégât reçu
    boss->isInvincible = false;
    return boss;
}

int displayBoss(SDL_Renderer *renderer, Boss *boss, Map *map, SDL_Texture *bossTexture, int showHitbox) {
    int c = 160; // côté du carré de destination du sprite du perso
    int centrage = 6;

    SDL_Rect dst_rect = {.x = boss->x * map->pix_rect - c / 2, .y = boss->y * map->pix_rect - c / 2 - centrage, .w = c, .h = c};
    SDL_Rect src_rect;
    if (!afficherImage && !parametre) {
        boss->spriteOffset = (boss->spriteOffset + 1) % 54; // 6 frames par sprite, 4 sprites
    }

    // Déterminer quel sprite afficher en fonction de la santé et de la gravité
    if (boss->health <= 3) {
        bossMove = BOSS_ANGRY_MOVE_INTERVAL;
        projectileSpeed = PROJECTILE_ANGRY_SPEED;
        src_rect = (SDL_Rect){.x = (boss->spriteOffset / 6) * 64, .y = 64, .w = 64, .h = 64};
        if (currentGravity < 0) {
            src_rect = (SDL_Rect){.x = (boss->spriteOffset / 6) * 64, .y = 3 * 64, .w = 64, .h = 64};
        }
    } else {
        src_rect = (SDL_Rect){.x = (boss->spriteOffset / 6) * 64, .y = 0, .w = 64, .h = 64};
        if (currentGravity < 0) {
            src_rect = (SDL_Rect){.x = (boss->spriteOffset / 6) * 64, .y = 2 * 64, .w = 64, .h = 64};
        }
        if (currentTime1 - lastProjectileLoad >= PROJECTILE_LOAD_INTERVAL - 500 && currentTime1 - lastProjectileLoad <= PROJECTILE_LOAD_INTERVAL + 500) {
            src_rect = (SDL_Rect){.x = (boss->spriteOffset / 6) * 64, .y = 64, .w = 64, .h = 64};
        }
        if (currentTime1 - lastProjectileLoad >= PROJECTILE_LOAD_INTERVAL - 500 && currentTime1 - lastProjectileLoad <= PROJECTILE_LOAD_INTERVAL + 500 && currentGravity < 0) {
            src_rect = (SDL_Rect){.x = (boss->spriteOffset / 6) * 64, .y = 3 * 64, .w = 64, .h = 64};
        }
    }
    if (!afficherImage && !parametre) {
        Uint32 currentTime = SDL_GetTicks();
        if (boss->isInvincible) {
            if (((currentTime + totalPauseDuration) / 250) % 2 == 0) {
                return 0;
            }
        }
    }

    // Rendre le boss si il n'est pas invincible ou quand il doit être affiché pendant le clignotement
    if (SDL_RenderCopyEx(renderer, bossTexture, &src_rect, &dst_rect, 0.0, NULL, SDL_FLIP_NONE)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
        exit(-1);
    }

    if (showHitbox) {
        if (display_boss_hitbox(renderer, boss, map)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in display perso hitbox: %s", SDL_GetError());
            exit(-1);
        }
    }

    return 0;
}

int display_boss_hitbox(SDL_Renderer *renderer, Boss *boss, Map *map) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect rect1 = {.x = boss->hitbox.x, .y = boss->hitbox.y, .w = boss->hitbox.w, .h = boss->hitbox.h};
    if (SDL_RenderDrawRect(renderer, &rect1)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in draw rect : %s", SDL_GetError());
		exit(-1);
    }
    return 0;
}

void loadBossTexture(SDL_Renderer *renderer, SDL_Texture **bossTexture, char *path) {
	SDL_Surface *bossSurface = IMG_Load(path);
	if (!bossSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in boss surface init %s", IMG_GetError());
		exit(-1);
	}
	*bossTexture = SDL_CreateTextureFromSurface(renderer, bossSurface);
	if (!*bossTexture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in boss texture init: %s", SDL_GetError());
		exit(-1);
	}
    // SDL_SetTextureScaleMode(*persoTexture, SDL_ScaleModeLinear);
	SDL_FreeSurface(bossSurface);
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
    Uint32 currentTime = SDL_GetTicks();
    if (boss->isInvincible && currentTime - boss->lastDamageTime >= 2000) { // Invincible pendant 2 secondes
        boss->isInvincible = false;
    }

    if (boss -> parcoursIndex >= 20) {
        boss->parcoursIndex = rand() % 8;
        boss -> currentMove = 0;
    }

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
    boss->currentMove++;
}

void damageBossBallon(Boss *boss) {
    Uint32 currentTime = SDL_GetTicks();
    if (!boss->isInvincible) {
        boss->health -= 1;
        boss->lastDamageTime = currentTime;
        boss->isInvincible = true;
        if (boss->health <= 0) {
            // Logique pour gérer la mort du boss
        }
    }
}
