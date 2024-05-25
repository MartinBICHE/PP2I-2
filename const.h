#include <SDL2/SDL.h>
#ifndef CONST_H
#define CONST_H
#include <stdbool.h>

extern bool showMenu;
extern bool afficherImage;
extern bool parametre;
extern bool retourMenu;
extern bool startGame;
extern bool prevShowMenu;
extern bool musicToggled;
extern bool quit;
extern bool running;
extern float currentGravity;
extern float jumpSpeed;
extern bool showAttentionImage;
extern bool firstIteration;
extern Uint32 lastGravityChange;
extern Uint32 lastProjectileLoad;
extern Uint32 lastBossMoveTime;
extern SDL_Event e;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* projectileTexture;
extern bool isBossMap;
extern Uint32 currentTime1;
extern Uint32 boutonGTime;
extern Uint32 pauseStartTime;
extern Uint32 totalPauseDuration;
extern const Uint8 *state;
extern Uint32 bossAngry1;
extern Uint32 bossAngry2;
extern Uint32 bossAngry3;
extern float bossMove;
extern float projectileSpeed;

#define Image1Width 200
#define Image1Height 150
#define ImageAttentionWidth 800
#define ImageAttentionHeight 500
#define BLUR_AMOUNT 0.5f
#define ImageRetourMenuWidth 200
#define ImageRetourMenuHeight 50
#define ImageQuitterJeuWidth 200
#define ImageQuitterJeuHeight 50
#define ImageParametrePauseWidth 50
#define ImageParametrePauseHeight 50
#define ImageRetourArrièreWidth 50
#define ImageRetourArrièreHeight 50
#define NUM_SPRITES 8

// Taille du curseur
#define CURSOR_WIDTH 20
#define CURSOR_HEIGHT 40
#define MAX_VOLUME 128

#define WINHEIGHT 720 // en pixels
#define WINWIDTH 1260 // en pixels
#define TIERWIDTH WINWIDTH/3
#define QUARTERHEIGHT WINHEIGHT/4
#define SPRITESIZE 64 // 64x64 taille des sprites de base

#define ACC 25.5f // Gravité 
#define ACC_INVERTED -25.5f // Gravité inversée

#define PERSO_WIDTH 0.8f // en nombre de tiles (la hitbox pas le sprite)
#define PERSO_HEIGHT 1.8f // en nombre de tiles (la hitbox pas le sprite)

#define DT 1/60.0f
#define JUMPSPEED 11.0f
#define JUMPSPEED_INVERTED -11.0f 
#define MOOVSPEED 7.0f
#define PROJECTILE_SPEED 150 // Vitesse du projectile (en nombre de tiles par frame)
#define PROJECTILE_ANGRY_SPEED 200 
#define MAX_PROJECTILES 8
#define PROJECTILE_WIDTH 50
#define PROJECTILE_HEIGHT 50
#define BOSS_SPEED 5
#define BOSS_WIDTH 2.5f // en nombre de tiles
#define BOSS_HEIGHT 3.2f // en nombre de tiles
#define BOSS_LEFT_LIMIT 0
#define BOSS_RIGHT_LIMIT 100
#define MAX_ROTATION_ANGLE 0.02f // Limite de rotation par mise à jour en radians
// #define INVINCIBILITY_FRAMES 1000

#define GRAVITY_CHANGE_INTERVAL 30000
#define PROJECTILE_LOAD_INTERVAL 5000
#define BOSS_MOVE_INTERVAL 2000
#define BOSS_ANGRY_MOVE_INTERVAL 1000

typedef struct _Map {
    char** matrix;
    int height; // en nombre de tiles
    int width; // en nombre de tiles
    int pix_rect; // en pixels
    int start_x; // en nombre de tiles
    int start_y; // en nombre de tiles
    int start_xboss;
    int start_yboss;
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
    int recoil_timer;
    int invincibility_timer;
    int jump_delay; // délai entre les sauts (22 frames)
    int dash_duration;
    float dash_speed;
    int dash_delay;
} Perso;

extern Perso* perso;
extern Map* map;

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

typedef struct {
    float x; 
    float y;
    float vx;
    float vy;
    bool active;
    SDL_Rect hitbox;
} Projectile;

extern Projectile projectiles[MAX_PROJECTILES]; // Tableau pour stocker les projectiles actifs

#endif
