#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include "enemyBat.h"
#include "graph.h"

/* s'utilise avec: */
  /* EnemyBatData enemyBatData; */
/*   initEnemyBat(&enemyBatData, xPos(à définir), yPos(à définir), xMax(à définir)); */
/* enemyBat_mouvement(renderer, &enemyBatData, map); */

void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Map *map, Perso *perso) {
    int interval = 130;
    int intervalAttack = 1000;
    int speed = 20;
    double position_tolerance = 20;
    SDL_Rect dst_rect = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    SDL_Rect dst_rect2 = {enemyBatData->dst_rectAttack.x - map->x_cam, enemyBatData->dst_rectAttack.y, 64*2, enemyBatData->dst_rectAttack.h};
    SDL_Rect dst_rect2Ex = {enemyBatData->dst_rectAttack.x - map->x_cam - 25, enemyBatData->dst_rectAttack.y, 64*2, enemyBatData->dst_rectAttack.h};
    int pauseInterval = 1000;
    /* printf("state: %d\n", enemyBatData->state); */
    printf("src rect height %d\n", enemyBatData->src_rect.h);
    printf("src rect width %d\n", enemyBatData->src_rect.w);
    printf("dst rect height %d\n", enemyBatData->dst_rect.h);
    printf("dst rect width %d\n", enemyBatData->dst_rect.w);

    int channel = Mix_PlayChannel(-1, musicEnemyBat, 0);
    double distance = sqrt(pow(perso->x * map->pix_rect - enemyBatData->dst_rect.x, 2) +
                       pow(perso->y * map->pix_rect - enemyBatData->dst_rect.y, 2));

    if (distance > MAX_HEARING_DISTANCE) {
        Mix_Volume(channel, MIN_VOLUME);
    } else {
        double attenuation = 5.0 / (distance - 100); 
        int volume = (int)(MAX_VOLUME * attenuation);
        Mix_Volume(channel, volume);
        printf("Adjusting volume to %d on channel %d based on distance %f\n", volume, channel, distance);
    }
    if (enemyBatData->state == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    if (enemyBatData->state == BAT_MOVING_LEFT){
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rect);
    }
    if (enemyBatData->state == BAT_ATTACK){
        if (enemyBatData->previousState == BAT_MOVING_LEFT){
            SDL_RenderCopy(renderer, textureBatAttack, &enemyBatData->src_rectAttack, &dst_rect2);
        }
        if (enemyBatData->previousState == BAT_MOVING_RIGHT){
            SDL_RenderCopyEx(renderer, textureBatAttack, &enemyBatData->src_rectAttack, &dst_rect2Ex, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }
    switch(enemyBatData->state){
        case BAT_MOVING_RIGHT:
            if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
                enemyBatData->dst_rect.x += speed;
                enemyBatData->src_rect.x += 32;
                enemyBatData->pauseStartBits = SDL_GetTicks();
            }
            if (enemyBatData->src_rect.x == 160){
                enemyBatData->src_rect.x = 0;
            }
            if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMax) <= position_tolerance){
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            break;
        case BAT_MOVING_LEFT:
        if (fabs((double)enemyBatData->dst_rect.x - enemyBatData->xMin) <= position_tolerance){
            enemyBatData->state = BAT_MOVING_RIGHT;
        }
        if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval){
            enemyBatData->dst_rect.x -= speed;
            enemyBatData->src_rect.x += 32;
            enemyBatData->pauseStartBits = SDL_GetTicks();
        }
        if (enemyBatData->src_rect.x == 160){
            enemyBatData->src_rect.x = 0;
        }
        break;
        case BAT_ATTACK:
        enemyBatData->dst_rectAttack.x = enemyBatData->dst_rect.x;
        enemyBatData->dst_rectAttack.y = enemyBatData->dst_rect.y;
            if (SDL_GetTicks() - enemyBatData->pauseStartBitsAttack >= interval){
                enemyBatData->src_rectAttack.x += 1*64;
                enemyBatData->pauseStartBitsAttack = SDL_GetTicks();
            }
            if (fabs((double)enemyBatData->src_rectAttack.x - 448) <= position_tolerance){
                enemyBatData->state = enemyBatData->previousState;
                enemyBatData->src_rectAttack.x = 0;
            }

        /* et le problème de musique pour tous les trucs */
        /* et les hitbox pour chaque personnage */

            break;
    }

}

int hitbox_enemyBat(Perso *perso, Map *map, EnemyBatData *enemy) {
    SDL_Rect enemyHitbox = enemy->dst_rect;
    int margin = 10; // Marge pour que le personnage ne soit pas collé à la hitbox de l'ennemi
    enemyHitbox.x -= margin;
    enemyHitbox.y -= margin;
    enemyHitbox.w += 2 * margin;
    enemyHitbox.h += 2 * margin;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}


void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax, Node *goal, Node *start, Map *map){

    enemyBatData->src_rect.x = 0; 
    enemyBatData->src_rect.y = 0; 
    enemyBatData->src_rect.w = 32;
    enemyBatData->src_rect.h = 64;

    /* enemyBatData->dst_rect.x = start->x * map->pix_rect; */
     /* enemyBatData->dst_rect.x = start->x * map->pix_rect; */
    enemyBatData->dst_rect.x = x;
    enemyBatData->dst_rect.y = y;
    enemyBatData->dst_rect.w = 32*2;
    enemyBatData->dst_rect.h = 64*2;

    enemyBatData->pauseStartBits = 0;
    enemyBatData->pauseStartBitsAttack = 0;
    enemyBatData->pauseAttack = 0;
    enemyBatData->state = BAT_MOVING_RIGHT;
    enemyBatData->xMax = xMax;
    enemyBatData->xMin = x;

    enemyBatData->src_rectAttack.x = 0; 
    enemyBatData->src_rectAttack.y = 0;
    enemyBatData->src_rectAttack.w = 64;
    enemyBatData->src_rectAttack.h = 64*2;

    enemyBatData->dst_rectAttack.x = x;
    enemyBatData->dst_rectAttack.y = y;
    enemyBatData->dst_rectAttack.w = 64*2;
    enemyBatData->dst_rectAttack.h = 64*2;

    
    if (goal != NULL) {
        enemyBatData->goal = goal;
    } else {
        enemyBatData->goal = NULL;
    }

    if (start != NULL) {
        enemyBatData->start = start;
    } else {
        enemyBatData->start = NULL;
    }

}


void batAttack(EnemyBatData *enemyBatData, Perso *perso, Map *map){
    int intervalAttack = 1000;
    int spriteLength = 64;

    if (enemyBatData->state != BAT_ATTACK){
        enemyBatData->previousState = enemyBatData->state;
    }

    if (hitbox_enemyBat(perso, map, enemyBatData)){
        if (SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack){
            perso->health -= 1;
            enemyBatData->pauseAttack = SDL_GetTicks();
            enemyBatData->pauseMusic = SDL_GetTicks();
            enemyBatData->state = BAT_ATTACK;
        }
    }
    /* if (SDL_GetTicks() - enemyBatData->pauseMusic >= intervalAttack){ */
    /*     Mix_HaltChannel(channelAttack); */
    /*     enemyBatData->pauseMusic = 0; */
    /* } */


       if (enemyBatData->state == BAT_ATTACK && SDL_GetTicks() - enemyBatData->pauseAttack >= intervalAttack) {
        enemyBatData->state = enemyBatData->previousState;
        enemyBatData->pauseAttack = SDL_GetTicks();
    }

}

void move_enemy_to_node(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node *node, Map *map) {
    enemyBatData->dst_rect.x = node->x * map->pix_rect;
    enemyBatData->dst_rect.y = node->y * map->pix_rect;
    printf("'%d, %d\n", node->x, node->y);
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
}


void follow_path(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node path[MAX_NODES], Map *map, Perso *perso) {
    int interval = 130;   
    int speed = 32;      
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    int path_length = len_nodes(path);

    int i;
    for (i = 0; i < path_length; i++) {
        if ((enemyBatData->dst_rect.x < path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_RIGHT) ||
            (enemyBatData->dst_rect.x > path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_LEFT)) {
            break;
        }
    }

    int channel = Mix_PlayChannel(-1, musicEnemyBat, 0);
    double distance = fabs(perso->x * map->pix_rect - enemyBatData->dst_rect.x);
    if (distance > MAX_HEARING_DISTANCE) {
        Mix_Volume(channel, MIN_VOLUME);
    } else {
        double attenuation = 5.0 / (distance - 100); 
        int volume = (int)(MAX_VOLUME * attenuation);
        Mix_Volume(channel, volume);
        printf("Adjusting volume to %d on channel %d based on distance %f\n", volume, channel, distance);
    }

    if (enemyBatData->state == BAT_MOVING_RIGHT) {
        SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else if (enemyBatData->state == BAT_MOVING_LEFT) {
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
    }

    int dx = path[i].x * map->pix_rect - enemyBatData->dst_rect.x;
    int dy = path[i].y * map->pix_rect - enemyBatData->dst_rect.y;
    printf("dx: %d\n", dx);

    if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval) {
        if (enemyBatData->state == BAT_MOVING_RIGHT) {
            if (enemyBatData->dst_rect.x / map->pix_rect >= path[i].x) {
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x += speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        } else if (enemyBatData->state == BAT_MOVING_LEFT) {
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x -= speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        }

        enemyBatData->src_rect.x += 32;
        if (enemyBatData->src_rect.x >= 160) {
            enemyBatData->src_rect.x = 0;
        }
        enemyBatData->pauseStartBits = SDL_GetTicks();
    }
}
void follow_path2(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node **graph, Map *map, Perso *perso) {
    int interval = 130;   
    int speed = 32;      
    Node *goal = &graph[2][144];
    Node *start = &graph[8][1];
    Node *path = a_star(graph, map, goal, start);
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    int path_length = len_nodes(path);

    int i;
    for (i = 0; i < path_length; i++) {
        if ((enemyBatData->dst_rect.x < path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_RIGHT) ||
            (enemyBatData->dst_rect.x > path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_LEFT)) {
            break;
        }
    }

    int channel = Mix_PlayChannel(-1, musicEnemyBat, 0);
    double distance = fabs(perso->x * map->pix_rect - enemyBatData->dst_rect.x);
    if (distance > MAX_HEARING_DISTANCE) {
        Mix_Volume(channel, MIN_VOLUME);
    } else {
        double attenuation = 5.0 / (distance - 100); 
        int volume = (int)(MAX_VOLUME * attenuation);
        Mix_Volume(channel, volume);
        printf("Adjusting volume to %d on channel %d based on distance %f\n", volume, channel, distance);
    }

    if (enemyBatData->state == BAT_MOVING_RIGHT) {
        SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else if (enemyBatData->state == BAT_MOVING_LEFT) {
        SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
    }

    int dx = path[i].x * map->pix_rect - enemyBatData->dst_rect.x;
    int dy = path[i].y * map->pix_rect - enemyBatData->dst_rect.y;
    printf("dx: %d\n", dx);

    if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval) {
        if (enemyBatData->state == BAT_MOVING_RIGHT) {
            if (enemyBatData->dst_rect.x / map->pix_rect >= path[i].x) {
                enemyBatData->state = BAT_MOVING_LEFT;
            }
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x += speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        } else if (enemyBatData->state == BAT_MOVING_LEFT) {
            if (abs(dx) >= speed) {
                enemyBatData->dst_rect.x -= speed;
            } else {
                enemyBatData->dst_rect.x = path[i].x * map->pix_rect;
            }
            if (abs(dy) >= speed) {
                enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
            } else {
                enemyBatData->dst_rect.y = path[i].y * map->pix_rect;
            }
        }

        enemyBatData->src_rect.x += 32;
        if (enemyBatData->src_rect.x >= 160) {
            enemyBatData->src_rect.x = 0;
        }
        enemyBatData->pauseStartBits = SDL_GetTicks();
    }
    free(path);
}



void enemyBat_follow(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node **graph, Map *map, Perso *perso){
    int interval = 130;   
    int speed = 20;      
    static Uint32 lastPathfindingTime = 0; 
    int delay = 2000;

      int xPos_perso = perso->x;
      int yPos_perso = perso->y;

      int xPos_bat = round(enemyBatData->dst_rect.x/map->pix_rect);
      int yPos_bat = round(enemyBatData->dst_rect.y/map->pix_rect);
      if (!(yPos_bat-1 >= 0 && yPos_perso-1 >= 0 && yPos_perso-1 <= map->height && yPos_bat-1 <= map->height && xPos_bat-1 >= 0 && xPos_perso-1 >= 0 && xPos_perso-1 <= map->width && xPos_bat-1 <= map->width)){
          puts("out of bounds");
          exit(-1);

      }

      Node *current_node_perso = &graph[yPos_perso-1][xPos_perso-1]; 
      Node *current_bat_node = &graph[yPos_bat-1][xPos_bat-1];
      if (!current_bat_node->walkable){
          current_bat_node = get_nearest_walkable_neighbor(graph, map, xPos_bat, yPos_bat);
          print_node(current_bat_node);
      }
      if (!current_node_perso->walkable){
          current_node_perso = get_nearest_walkable_neighbor(graph, map, xPos_perso, yPos_perso);
          print_node(current_bat_node);
      }
      /* current_bat_node->walkable = true; */
      /* current_node_perso->walkable = true; */
      print_node(current_bat_node);
      /* static Node *path = NULL; */
      if (SDL_GetTicks() - lastPathfindingTime >= delay){
        if (enemyBatData->state == BAT_MOVING_RIGHT) {
            enemyBatData->path = a_star(graph, map, current_node_perso, current_bat_node);
        } else {
            enemyBatData->path = a_star(graph, map, current_bat_node, current_node_perso);
        }
        lastPathfindingTime = SDL_GetTicks();
      }
    SDL_Rect dst_rectFixed = {enemyBatData->dst_rect.x - map->x_cam, enemyBatData->dst_rect.y, enemyBatData->dst_rect.w, enemyBatData->dst_rect.h};
    if (enemyBatData->path != NULL){
        int path_length = len_nodes(enemyBatData->path);

        int i;
        for (i = 0; i < path_length; i++) {
            if ((enemyBatData->dst_rect.x < enemyBatData->path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_RIGHT) ||
                (enemyBatData->dst_rect.x > enemyBatData->path[i].x * map->pix_rect && enemyBatData->state == BAT_MOVING_LEFT)) {
                break;
            }
        }
        if (enemyBatData->state == BAT_MOVING_RIGHT) {
            SDL_RenderCopyEx(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else if (enemyBatData->state == BAT_MOVING_LEFT) {
            SDL_RenderCopy(renderer, textureBat, &enemyBatData->src_rect, &dst_rectFixed);
        }
        printf("state %d\n", enemyBatData->state);

        int dx = enemyBatData->path[i].x * map->pix_rect - enemyBatData->dst_rect.x;
        int dy = enemyBatData->path[i].y * map->pix_rect - enemyBatData->dst_rect.y;

        if (SDL_GetTicks() - enemyBatData->pauseStartBits >= interval) {
            if (enemyBatData->state == BAT_MOVING_RIGHT) {
                if (current_node_perso->x <= current_bat_node->x ) {
                    enemyBatData->state = BAT_MOVING_LEFT;
                }
                if (abs(dx) >= speed) {
                    enemyBatData->dst_rect.x += speed;
                } else {
                    enemyBatData->dst_rect.x = enemyBatData->path[i].x * map->pix_rect;
                }
                if (abs(dy) >= speed) {
                    enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
                } else {
                    enemyBatData->dst_rect.y = enemyBatData->path[i].y * map->pix_rect;
                }
            } else if (enemyBatData->state == BAT_MOVING_LEFT) {
                if (current_node_perso->x > current_bat_node->x){
                    enemyBatData->state = BAT_MOVING_RIGHT;
                }
                if (abs(dx) >= speed) {
                    enemyBatData->dst_rect.x -= speed;
                } else {
                    enemyBatData->dst_rect.x = enemyBatData->path[i].x * map->pix_rect;
                }
                if (abs(dy) >= speed) {
                    enemyBatData->dst_rect.y += speed * (dy > 0 ? 1 : -1);
                } else {
                    enemyBatData->dst_rect.y = enemyBatData->path[i].y * map->pix_rect;
                }
            }

            enemyBatData->src_rect.x += 32;
            if (enemyBatData->src_rect.x >= 160) {
                enemyBatData->src_rect.x = 0;
            }
            enemyBatData->pauseStartBits = SDL_GetTicks();
        }
    }

        /* free(path); */
}

