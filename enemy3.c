#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"
#include "enemy3.h"
#include "textures.h"
#include "music.h"

/* s'utilise avec : */
/*     enemy3_movement(renderer, &enemy, map); */
/*     Enemy3 enemy; */
/*     initEnemy3(&enemy, xPos(à définir), yPos(à définir), xCollisionMax(à définir)) */

void initEnemy3(Enemy3 *enemy, int x, int y, int xCollisionMax){
    enemy->src_rect.x = 0;
    enemy->src_rect.y = 0;
    enemy->src_rect.w = 64;
    enemy->src_rect.h = 64;

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
}



void enemy3_movement(SDL_Renderer *renderer, Enemy3 *enemy, Map *map) {
    Uint32 ticks = SDL_GetTicks();
    int frameNumber = 9;
    /* int frameNumberAttack = 9; */
    Uint32 sprite = (ticks / 400) % frameNumber;
    int intervalAttack = 1000;

    SDL_Rect dst_rectFixed = {enemy->dst_rect.x - map->x_cam, enemy->dst_rect.y, enemy->dst_rect.w, enemy->dst_rect.h};

    switch (enemy->state) {
        case LEFT:
            enemy->xPosition -= enemy->speed * 0.1;
            if (enemy->xPosition <= enemy->xCollisionMin) {
                enemy->state = RIGHT;
            }
            break;
        case RIGHT:
            enemy->xPosition += enemy->speed * 0.1;
            if (enemy->xPosition >= enemy->xCollisionMax) {
                enemy->state = LEFT;
            }
            break;

        case ATTACK:
            if (SDL_GetTicks() - enemy->pauseAttack >= intervalAttack){
                enemy->state = RIGHT;
            }
            /* je laisse ça comme ça pour l'instant à voir par la suite à mettre de manière à ce que l'animation commence au début et à bouger dès que l'animation prend fin*/

            /* if (sprite == 8){ */
            /*     enemy->state = RIGHT; */
            /* } */
            /* Uint32 ticksAnimationAttack = SDL_GetTicks(); */
            /* Uint32 spriteAttack = (ticks / 400) % 8; */

            break;
    }

    enemy->src_rect.x = sprite * 64;
    enemy->dst_rect.x = enemy->xPosition;

    switch (enemy->state) {
        case LEFT:
            SDL_RenderCopy(renderer, textureEnemy3, &enemy->src_rect, &dst_rectFixed);
            break;
        case RIGHT:
            SDL_RenderCopyEx(renderer, textureEnemy3, &enemy->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
            break;
        case ATTACK:
            SDL_RenderCopy(renderer, textureEnemy1, &enemy->src_rect, &dst_rectFixed);
            break;
    }
}

void enemy3Attack(Enemy3 *enemy, Perso *perso, Map *map){
    int intervalAttack = 1000;
    /* printf("perso->x %f\n", perso->x * map->pix_rect); */
    /* printf("enemy->dst_rect.x %d\n", enemy->dst_rect.x); */
    /* à laisser ces commentaires, ça aide à debug après, svp )) */
    int spriteLength = 64;
    float borneInf = enemy->dst_rect.x;
    float borneSup = borneInf + spriteLength;
    float persoPositionX = perso->x * map->pix_rect;

    /* à faire des trucs pour pouvoir sauter aussi avec la position y*/

    if (persoPositionX >= borneInf && persoPositionX <= borneSup){
        if (SDL_GetTicks() - enemy->pauseAttack >= intervalAttack && perso->health >= 0){
            perso->health -= 1;
            enemy->pauseAttack = SDL_GetTicks();
            enemy->state = ATTACK;
            Mix_PlayMusic(musicEnemySnake, -1);
        }
    }
    if (SDL_GetTicks() - enemy->pauseMusic >= intervalAttack){
        Mix_HaltMusic();
        enemy->pauseMusic = SDL_GetTicks();
    }
}


