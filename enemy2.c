#include "enemy2.h"
#include "perso.h"



/* Pour l'utiliser: */
/* Avant le while(running); */
/* Enemy2 enemy; */
/* Node *goal = &graph[13 (goal_y)][24 (goal_x)];  le premier paramère va jusqu'à map->height - 1 et l'autre jusqu'à map->width -1, et il faut que ça soit pas un obstacle */ 
/* Node *startA = &graph[10 (start_y)][15 (start_x)]; */ 
/* initEnemy2(&enemy, startEnemy, goalEnemy, map); */
/* puis juste avant le render_present: */
/* enemy2_follow(renderer, &enemy, graph, map); */
/* pour l'attaque: */
/* enemy2Attack(&enemy, perso, map); */

void enemy2_follow(SDL_Renderer *renderer, Enemy2 *enemy, Node **graph, Map *map){
    int interval = 130;   
    int speed = 20;      
      Node *path;
    if (enemy->state == EYE_MOVING_RIGHT) {
        path = a_star(graph, map, enemy->goal, enemy->start);
    } else {
        path = a_star(graph, map, enemy->start, enemy->goal);
    }
    SDL_Rect dst_rectFixed = {enemy->dst_rect.x - map->x_cam, enemy->dst_rect.y, enemy->dst_rect.w, enemy->dst_rect.h};
    int path_length = len_nodes(path);

    int i;
    for (i = 0; i < path_length; i++) {
        if ((enemy->dst_rect.x < path[i].x * map->pix_rect && enemy->state == EYE_MOVING_RIGHT) ||
            (enemy->dst_rect.x > path[i].x * map->pix_rect && enemy->state == EYE_MOVING_LEFT)) {
            break;
        }
    }
    if (enemy->state == EYE_MOVING_RIGHT) {
        SDL_RenderCopyEx(renderer, textureEnemy2, &enemy->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else if (enemy->state == EYE_MOVING_LEFT) {
        SDL_RenderCopy(renderer, textureEnemy2, &enemy->src_rect, &dst_rectFixed);
    }

    int dx = path[i].x * map->pix_rect - enemy->dst_rect.x;
    int dy = path[i].y * map->pix_rect - enemy->dst_rect.y;

    if (SDL_GetTicks() - enemy->pauseStartBits >= interval) {
        if (enemy->state == EYE_MOVING_RIGHT) {
            if (enemy->dst_rect.x >= enemy->goal->x * map->pix_rect) {
                enemy->state = EYE_MOVING_LEFT;
            }
            if (abs(dx) >= speed) {
                enemy->dst_rect.x += speed;
            } else {
                enemy->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemy->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemy->dst_rect.y = path[i].y * map->pix_rect;
            }
        } else if (enemy->state == EYE_MOVING_LEFT) {
            if (enemy->dst_rect.x <= enemy->start->x * map->pix_rect){
                enemy->state = EYE_MOVING_RIGHT;
            }
            if (abs(dx) >= speed) {
                enemy->dst_rect.x -= speed;
            } else {
                enemy->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemy->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemy->dst_rect.y = path[i].y * map->pix_rect;
            }
        }

        enemy->src_rect.x += 64;
        if (enemy->src_rect.x >= 512) {
            enemy->src_rect.x = 0;
        }
        enemy->pauseStartBits = SDL_GetTicks();
    }
    free(path);
}


void initEnemy2(Enemy2 *enemy, Node *start, Node *goal, Map *map){
    if (!goal->walkable || !start->walkable){
        puts("Error: the goal or the start is not walkable");
        exit(-1);
    }
    enemy->src_rect.x = 0;
    enemy->src_rect.y = 0;
    enemy->src_rect.w = 64;
    enemy->src_rect.h = 64;

    enemy->dst_rect.x = start->x * map->pix_rect;
    enemy->dst_rect.y = start->y * map->pix_rect;
    enemy->dst_rect.w = 64;
    enemy->dst_rect.h = 64;

    enemy->start = start;
    enemy->goal = goal;
    enemy->pauseAttack = 0;
    enemy->health = 2;
}

void enemy2Attack(Enemy2 *enemy, Perso *perso, Map *map) {
  int intervalAttack = 1000;
  if (hitbox_enemy2(perso, map, enemy)){
      if (SDL_GetTicks() - enemy->pauseAttack >= intervalAttack){
          if (perso->health > 0){
              perso->health -= 1;
              enemy->pauseAttack = SDL_GetTicks();
              /* à mettre effet sonore et animation */
          }
      }
  }
}

int hitbox_enemy2(Perso *perso, Map *map, Enemy2 *enemy) {
    SDL_Rect enemyHitbox = enemy->dst_rect;
    int margin = 30; // Marge pour que le personnage ne soit pas collé à la hitbox de l'ennemi
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

void updatePersoEnemy2(Perso *perso, Map *map, Enemy2 *enemy){
    if (!isBossMap){
        if (hitbox_enemy2(perso, map, enemy)){
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

