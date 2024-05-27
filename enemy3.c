#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "enemy3.h"
#include "textures.h"
#include "music.h"
#include "perso.h"

/* s'utilise avec : */
/*     enemy3_movement(renderer, &enemy, map); */
/*     Enemy3 enemy; */
/*     initEnemy3(&enemy, xPos(à définir), yPos(à définir), xCollisionMax(à définir)) */

void initEnemy3(Enemy3 *enemy, int x, int y, int xCollisionMax){
    enemy->src_rect.x = 0;
    enemy->src_rect.y = 0;
    enemy->src_rect.w = 64;
    enemy->src_rect.h = 64;

    enemy->src_rectAttack.x = 0;
    enemy->src_rectAttack.y = 0;
    enemy->src_rectAttack.w = 64;
    enemy->src_rectAttack.h = 64;

    enemy->dst_rect.x = x;
    enemy->dst_rect.y = y;
    enemy->dst_rect.w = 64;
    enemy->dst_rect.h = 64;

    enemy->xPosition = enemy->dst_rect.x;
    enemy->yPosition = enemy->dst_rect.y;
    enemy->speed = 10;
    enemy->xCollisionMax = xCollisionMax;
    enemy->xCollisionMin = enemy->dst_rect.x;
    enemy->dx = 1;
    enemy->pauseAttack = 0;
    enemy->pauseBits = 0;
}

int hitbox_enemy3(Perso *perso, Map *map, Enemy3 *enemy) {
    SDL_Rect enemyHitbox = enemy->dst_rect;
    int margin = 10; // Marge pour que le personnage ne soit pas collé à la hitbox de l'ennemi
    enemyHitbox.x -= margin;
    enemyHitbox.y -= margin;
    enemyHitbox.w += round(1.0 * margin);
    enemyHitbox.h += round(1.0 * margin);
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}

void updatePersoEnemy3(Perso *perso, Map *map, Enemy3 *enemy){
    if (!isBossMap){
        if (hitbox_enemy3(perso, map, enemy)){
            float dx = perso->vx * DT;
            float dy = perso->vy * DT;
            if (dx > 0) { // Le personnage se déplace vers la droite
                perso->vx = max(perso->vx, 0.0f);
                // Position juste avant le début de la hitbox de l'ennemi (côté gauche)
                perso->x = enemy->dst_rect.x / map->pix_rect - PERSO_WIDTH / 2.0f + 0.5;
            } else if (dx < 0) { // Le personnage se déplace vers la gauche
                perso->vx = min(perso->vx, 0.0f);
                // Position juste avant le début de la hitbox de l'ennemi (côté droit)
                perso->x = (enemy->dst_rect.x + enemy->dst_rect.w) / map->pix_rect + PERSO_WIDTH / 2.0f + 0.3;
            }
            if (dy > 0) { // Le personnage se déplace vers le bas
                // Faire rebondir le personnage au dessus de l'ennemi
                perso->vy = -JUMPSPEED;
        }
        }
    }
}




void enemy3_movement(SDL_Renderer *renderer, Enemy3 *enemy, Map *map) {
    Uint32 ticks = SDL_GetTicks();
    int frameNumber = 9;
    Uint32 sprite = (ticks / 400) % frameNumber;
    int bitsSpeed = 100;
    double position_tolerance = 10;
    int interval = 1000;

    SDL_Rect dst_rectFixed = {enemy->dst_rect.x - map->x_cam, enemy->dst_rect.y, enemy->dst_rect.w, enemy->dst_rect.h};

    switch (enemy->state) {
        case LEFT:
            if (SDL_GetTicks() - enemy->pauseBits >= interval){
                enemy->src_rect.x += 64;
                enemy->pauseBits = SDL_GetTicks();
            }
            enemy->xPosition -= enemy->speed * 0.1;
            if (enemy->xPosition <= enemy->xCollisionMin) {
                enemy->state = RIGHT;
            }
            if (enemy->src_rect.x == 512){
                enemy->src_rect.x = 0;
            }
            break;
        case RIGHT:
            if (SDL_GetTicks() - enemy->pauseBits >= interval){
                enemy->src_rect.x += 64;
                enemy->pauseBits = SDL_GetTicks();
            }
            enemy->xPosition += enemy->speed * 0.1;
            if (enemy->xPosition >= enemy->xCollisionMax) {
                enemy->state = LEFT;
            }
            if (enemy->src_rect.x == 512){
                enemy->src_rect.x = 0;
            }
            break;

        case ATTACK:
            if (SDL_GetTicks() - enemy->pauseAttack >= bitsSpeed){
                enemy->src_rectAttack.x += 64;
                enemy->pauseAttack = SDL_GetTicks();
            }
            if (fabs((double)enemy->src_rectAttack.x - 448 ) <= position_tolerance){
                enemy->state = enemy->previousState;
                enemy->src_rectAttack.x = 0;
            }
            break;
    }

    /* enemy->src_rect.x = sprite * 64; */
    enemy->dst_rect.x = enemy->xPosition;

    switch (enemy->state) {
        case LEFT:
            SDL_RenderCopy(renderer, textureEnemy3, &enemy->src_rect, &dst_rectFixed);
            break;
        case RIGHT:
            SDL_RenderCopyEx(renderer, textureEnemy3, &enemy->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
            break;
        case ATTACK:
            if (enemy->previousState == LEFT){
                SDL_RenderCopy(renderer, textureEnemy3Attack, &enemy->src_rectAttack, &dst_rectFixed);
            }
            if (enemy->previousState == RIGHT){
                SDL_RenderCopyEx(renderer, textureEnemy3Attack, &enemy->src_rectAttack, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
            /* int volume = MAX_VOLUME; */
            /* int channel = Mix_PlayChannel(-1, musicEnemyBat, 0); */
            /* Mix_Volume(channel, volume); */
            break;
    }
}

void enemy3Attack(Enemy3 *enemy, Perso *perso, Map *map){
    int intervalAttack = 1000;
    if (enemy->state != ATTACK){
        enemy->previousState = enemy->state;
    }

    if (hitbox_enemy3(perso, map, enemy)){
        if (SDL_GetTicks() - enemy->pauseAttack >= intervalAttack && perso->health >= 0){
            perso->health -= 1;
            enemy->pauseAttack = SDL_GetTicks();
            enemy->state = ATTACK;
        }
    }
}

