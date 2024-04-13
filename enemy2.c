#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "const.h"
#include "main.h"
#include "enemy2.h"
#include "map.h"
#include "display.h"


MoveList *newMoveList(MoveData data){
    MoveList *temp = malloc(sizeof(MoveList));
    temp->mouvement = data;
    temp->next = NULL;
    return temp;
}

void insertAtEnd( MoveList** head, MoveData data) {
  MoveList * new_node = newMoveList(data);

  if (*head == NULL) {
    *head = new_node;
    new_node->next = new_node;
  } else {
    // Find the last node
    MoveList* last = *head;
    while (last->next != *head) {
      last = last->next;
    }
    last->next = new_node;
    new_node->next = *head;
  }
}






MoveList *rightMovementList(const int xInitialPosition, const int yInitialPosition, Map map, int xEndPosition){

    MoveList *res = NULL;
    static int yPosition = 0;
    if (yPosition == 0){
        yPosition = yInitialPosition;
    }
    for (int i = xInitialPosition; i <= xEndPosition; i ++){
        if (strncmp(&map.matrix[yPosition+1][i], "-", 1) == 0 ) {
            yPosition += 1;
            MoveData *temp = malloc(sizeof(MoveData));
            temp->jumpDirection = 1;
            temp->position = i;
            insertAtEnd(&res, *temp) ;
            free(temp);
        }
        if (!(strncmp(&map.matrix[yPosition][i+1], "-", 1) == 0)){
            yPosition -= 1;
            MoveData *temp = malloc(sizeof(MoveData));
            temp->jumpDirection = -1;
            temp->position = i+1;
            insertAtEnd(&res, *temp) ;
            free(temp);
        }
    }
    return res;
}

void traverse(MoveList *last) {
    printf("-------");

    MoveList *p;
    if (last == NULL) {
        printf("The list is empty");
        return;
    }

  p = last->next;

  do {
      printf("(");
      printf("%d ", p->mouvement.position);
      printf("%d ", p->mouvement.jumpDirection);
      printf(")");
      p = p->next;
  } while (p != last->next);
}



MoveList *leftMovementList(const int xInitialPosition, const int yInitialPosition, Map map, int xEndPosition){
    MoveList *res = NULL;
    static int yPosition = 0;
    if (yPosition == 0){
        yPosition = yInitialPosition;
    }
    for (int i = xEndPosition; i >= xInitialPosition; i--){
        if (!(strncmp(&map.matrix[yPosition][i+1], "-", 1) == 0)){
            yPosition -= 1;
            MoveData *temp = malloc(sizeof(MoveData));
            temp->jumpDirection = -1;
            temp->position = i+1;
            insertAtEnd(&res, *temp);
            free(temp);
        }
        if (strncmp(&map.matrix[yPosition+1][i], "-", 1) == 0 ) {
            yPosition += 1;
            MoveData *temp = malloc(sizeof(MoveData));
            temp->jumpDirection = +1;
            temp->position = i;
            insertAtEnd(&res, *temp);
            free(temp);
        }
    }
    return res;
}

void concatenate(MoveList** head1, MoveList** head2) {
  if (*head1 == NULL) {
    *head1 = *head2;
    return;
  }

  if (*head2 == NULL) {
    return;
  }

  MoveList *last1 = (*head1)->next;
  while (last1->next != *head1) {
    last1 = last1->next;
  }

  MoveList *last2 = (*head2)->next;
  while (last2->next != *head2) {
    last2 = last2->next;
  }
  last1->next = *head2;
  last2->next = *head1;
}


void enemy2_movement(SDL_Renderer *renderer, SDL_Texture *texture, Enemy2 *enemy, float scale, Map *map, MoveList **list_ptr){
    static int dx = 1;
    enemy->xPosition += dx *enemy->speed * 0.1;
    MoveList *list = *list_ptr;
    render_sprite(renderer, texture, enemy->xPosition, enemy->yPosition, scale, enemy->image_path);

    int position_tolerance = 3;

    if (fabs(enemy->xPosition - list->mouvement.position *PIX_RECT) <= position_tolerance){
        enemy->yPosition += PIX_RECT*list->mouvement.jumpDirection;
        list = list->next;
        *list_ptr = list;
    }
    if (enemy->xPosition <= enemy->collision_min_x-30 || enemy->xPosition >= enemy->collision_max_x -30){
        dx *= -1;
        list = list->next;
        *list_ptr = list;
        if (dx == -1){
            enemy->yPosition = enemy->yInitialPosition*PIX_RECT;
        }
        if (dx == 1){
            enemy->yPosition = (enemy->yInitialPosition + 1)*PIX_RECT;
        }
    }
}

