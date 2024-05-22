#ifndef BOSS_H
#define BOSS_H

#include "const.h"
#include "map.h"
#include <SDL2/SDL.h>

// Structure du boss
typedef struct _Boss {
    float x; // en nombre de tiles
    float y; // en nombre de tiles
    float vx; 
    float vy;
    SDL_Rect hitbox;
    Uint32 lastMoveTime; // Temps du dernier mouvement
    int parcoursIndex;
    int direction;
    int currentMove;
} Boss;

// Fonctions
Boss* create_boss(Map* map);
void updateBoss(Boss* boss, Map* map);
int displayBoss(SDL_Renderer* renderer, Boss* boss, Map* map);
int hitboxRightBoss(Boss *boss, Map *map);
int hitboxLeftBoss(Boss *boss, Map *map);
int hitboxTopBoss(Boss *boss, Map *map);
int hitboxBottomBoss(Boss *boss, Map *map);


#endif /* BOSS_H */
