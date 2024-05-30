#ifndef PENDULE_H
#define PENDULE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.h"
#include "perso.h"
#define INIT_ENEMYPENDULE(enemy, map, x, y) initEnemyPendule(enemy, (x) * (map)->pix_rect, (y) * (map)->pix_rect)



enum EnemyPenduleState{
    PENDULE_MOVING_UP,
    PENDULE_MOVING_DOWN,
    PENDULE_PAUSE_TOP,
    PENDULE_PAUSE_BOTTOM,
};

struct _EnemyPenduleData{
    enum EnemyPenduleState state;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    Uint32 pauseStartBits;
    Uint32 pauseStart;
    Uint32 pauseAttack;
    int attackCounter;
    Uint32 pauseMusic;

};

typedef struct _EnemyPenduleData EnemyPenduleData;
void enemyPendule_mouvement(SDL_Renderer *renderer, EnemyPenduleData *enemyFlecheData, Map *map);
void initEnemyPendule(EnemyPenduleData *enemyPenduleData, int x, int y);
int hitbox_enemyPendule(Perso *perso, Map *map, EnemyPenduleData *enemy);
void penduleAttack(EnemyPenduleData *enemy, Perso *perso, Map *map);
void updatePersoPendule(Perso *perso, Map *map, EnemyPenduleData *enemy);

#endif
