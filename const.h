#include <SDL2/SDL.h>
#ifndef CONST_H
#define CONST_H

#define WINHEIGHT 720 // en pixels
#define WINWIDTH 1280 // en pixels
#define TIERWIDTH WINWIDTH/3
#define QUARTERHEIGHT WINHEIGHT/4
#define SPRITESIZE 64 // 64x64 taille des sprites de base
#define PERSO_WIDTH 0.8f // en nombre de tiles (la hitbox pas le sprite)
#define PERSO_HEIGHT 1.8f // en nombre de tiles (la hitbox pas le sprite)
#define ACC 25.5f
#define DT 1/60.0f
#define JUMPSPEED 11.0f
#define MOOVSPEED 7.0f


typedef struct _Map {
    char** matrix;
    int height; // en nombre de tiles
    int width; // en nombre de tiles
    int pix_rect; // en pixels
    int start_x; // en nombre de tiles
    int start_y; // en nombre de tiles
    int end_x; // en nombre de tiles
    int end_y; // en nombre de tiles
    float x_cam; // en pixels
} Map ;


typedef struct _Perso {
    float x; // en nombre de tiles
    float y; // en nombre de tiles
    float vx; // en nombre de tiles par seconde
    float vy; // en nombre de tiles par seconde
    int facing; // -1 : gauche ; 1 : droite
    SDL_Rect hitbox;
    int spriteOffset; // décalage du sprite dans la spritesheet
    int health; // je l'ai rajouté
    int jumps; // nombre de sauts restants
    int jump_delay; // délai entre les sauts (22 frames)
    int dash_duration;
    float dash_speed;
    int dash_delay;
} Perso;


/* Struct dédié au gameplay 2, on les reconnait au label fight */

typedef struct _PersoFight {
    float x; // Position du joueur
    float y; // Position du joueur
    int health; // Points de vie du joueur
    int iframe; // Temps d'invincibilité après avoir été touché
} PersoFight;

typedef struct _AttackFight {
    int x; // Position de l'attaque
    int y;
    int warning; // Position de l'attaque
    int delay; // Temps avant la prochaine attaque
    int hitPoint; // Si l'attaque est un point pour infliger des dégats au boss
} AttackFight;

typedef struct _bossFight {
    int health; // Points de vie du boss
    int phase; // Chaque phase a des patterns différent et des vitesses différentes
    int delay; // Temps avant la prochaine attaque
    int attack1Delay;
    int attack2Delay;
    int attack3Delay;
    int speed;  // Le vitesse d'enchainement des attaques
} bossFight;


#endif

