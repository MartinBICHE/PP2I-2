#ifndef ENEMYBAT_H
#define ENEMYBAT_H
#include "enemyFleche.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include "graph.h"
#include "music.h"

enum EnemyBatState{
    BAT_MOVING_RIGHT,
    BAT_MOVING_LEFT,
    BAT_ATTACK
};

struct _EnemyBatData{
    enum EnemyBatState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    SDL_Rect src_rectAttack;
    SDL_Rect dst_rectAttack;
    Uint32 pauseStartBits;
    Uint32 pauseStartBitsAttack;
    int xMax;
    int xMin;
    Uint32 pauseAttack;
    Uint32 pauseMusic;
    Node *goal;
    Node *start;
    Node *path;

    enum EnemyBatState previousState;
};
typedef struct _EnemyBatData EnemyBatData;
void enemyBat_mouvement(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Map *map, Perso *perso);
void batAttack(EnemyBatData *enemyBatData, Perso *perso, Map *map);
void enemyBat_mouvement2(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node **pathList, Map *map);
void move_enemy_smoothly(SDL_Renderer *renderer, EnemyBatData *enemy, Node **path, int path_length, float speed, Map *map);
void move_enemy_to_node(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node *node, Map *map);
void follow_path(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node path[MAX_NODES], Map *map, Perso *perso);
void follow_path2(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node **graph, Map *map, Perso *perso);
void enemyBat_follow(SDL_Renderer *renderer, EnemyBatData *enemyBatData, Node **graph, Map *map, Perso *perso);
void initEnemyBat(EnemyBatData *enemyBatData, int x, int y, int xMax, Node *goal, Node *start, Map *map);
int hitbox_enemyBat(Perso *perso, Map *map, EnemyBatData *enemy);
#endif
