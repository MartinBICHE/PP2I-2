#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "const.h"
#include "perso.h"


Perso *create_perso(Map *map) {
    Perso *res = malloc(sizeof(Perso)) ;
    res->x = map->start_x ;
    res->y = map->start_y ;
    res->vx = 0 ;
    res->vy = 0 ;
    return res ;
}

int display_perso(SDL_Renderer *renderer, Perso *perso) {
    const SDL_Rect rect = {.x = perso->x - 10, .y = perso->y - 10, .w = 20, .h = 20} ;
    SDL_RenderDrawRect(renderer, &rect) ;
    return 0 ;
}


// void update(Perso *perso) {

// }