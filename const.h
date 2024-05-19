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

#define cursorX WINWIDTH / 2 - CURSOR_WIDTH / 2
#define cursorY WINHEIGHT / 2 - CURSOR_HEIGHT / 2
// #define PIX_RECT 30
// #define WINWIDTH WIDTH*PIX_RECT
// #define WINHEIGHT HEIGHT*PIX_RECT
// #define MENU_WINDOW_WIDTH 1400
// #define MENU_WINDOW_HEIGHT 800
#define Image1Width 200
#define Image1Height 150
#define ImageAttentionWidth 800
#define ImageAttentionHeight 500
// #define LOADING_WINDOW_WIDTH 600
// #define LOADING_WINDOW_HEIGHT 400
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

// Coordonnées du curseur
// #define cursorX WINWIDTH / 2 - CURSOR_WIDTH / 2;
// #define cursorY WINHEIGHT / 2 - CURSOR_HEIGHT / 2;


#define WINHEIGHT 720 // en pixels
#define WINWIDTH 1260 // en pixels
#define TIERWIDTH WINWIDTH/3
#define QUARTERHEIGHT WINHEIGHT/4
#define SPRITESIZE 64 // 64x64 taille des sprites de base
#define PERSO_WIDTH 0.66666f // en nombre de tiles
#define PERSO_HEIGHT 1.33333f // en nombre de tiles
#define ACC 25.5f // Gravité 
#define ACC_INVERTED -25.5f // Gravité inversée
#define DT 1/60.0f
#define JUMPSPEED 11.0f
#define JUMPSPEED_INVERTED -11.0f 
#define MOVSPEED 7.0f
#define PROJECTILE_SPEED 150 // Vitesse du projectile (en nombre de tiles par frame)
#define MAX_PROJECTILES 3
#define PROJECTILE_WIDTH 50
#define PROJECTILE_HEIGHT 50
#define BOSS_SPEED 5
#define BOSS_WIDTH 1.33333f // en nombre de tiles
#define BOSS_HEIGHT 2.66666f // en nombre de tiles
#define BOSS_LEFT_LIMIT 0
#define BOSS_RIGHT_LIMIT 100
#define MAX_ROTATION_ANGLE 0.02f // Limite de rotation par mise à jour en radians

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
    int health; //je l'ai rajouté
    SDL_Rect hitbox;
    int jumps; // nombre de sauts restants
    int jump_delay; // délai entre les sauts (25 frames)
    int recoil_timer;
} Perso;

extern Perso* perso;
extern Map* map;

// Structure de données représentant un projectile
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
