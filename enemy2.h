#ifndef enemy2_h
#define enemy2_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "map.h"
#include "const.h"

struct _MoveData{
    int jumpDirection;  
    int position;
};
typedef struct _MoveData MoveData;


struct _MoveList{
    MoveData mouvement;
    struct _MoveList *next;
};
typedef struct _MoveList MoveList;


struct _Enemy2{
    char *image_path;
    int speed;
    float xPosition;
    float yPosition;
    int health;
    int collision_min_x;
    int collision_max_x;
    int collision_min_y;
    int collision_max_y;
    int xInitialPosition;
    int yInitialPosition;
};
typedef struct _Enemy2 Enemy2;


MoveList *newMoveList(MoveData data);
void insertAtEnd(MoveList **head, MoveData data);
void printList(MoveList* head);
void traverse(MoveList *last);
void enemy2_movement(SDL_Renderer *renderer, SDL_Texture *texture, Enemy2 *enemy, float scale, Map *map, MoveList **list_ptr);
void concatenate(MoveList** head1, MoveList** head2);
MoveList *rightMovementList(const int xInitialPosition, const int yInitialPosition, Map map, int xEndPosition);
MoveList *leftMovementList(const int xInitialPosition, const int yInitialPosition, Map map, int xEndPosition);

#endif
