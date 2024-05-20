#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "enemyBat.h"
#include "enemyFleche.h"
#include "fight.h"
#include "fonts.h"
#include "health.h"
#include "init.h"
#include "map.h"
#include "pendule.h"
#include "perso.h"
#include "scroll.h"
#include "textures.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
/* #include "music.h" */
#include "graph.h"

SDL_Texture *bgTextures[6];
SDL_Texture *tileTextures;

int main(int argc, char **argv) {

    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

    Perso *playerInFight = (Perso *)malloc(sizeof(Perso));
    playerInFight->y = QUARTERHEIGHT - SPRITESIZE / 2;
    playerInFight->x = TIERWIDTH / 2 - SPRITESIZE / 2;
    Map *map = initMap("map2");
    Perso *perso = create_perso(map);

    SDL_Event event;
    int running = 1;

    loadBackgroundTextures(renderer, bgTextures, 5);
    loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");
    /* const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255}; */ 

  EnemyStateData enemyStateData;
  initEnemy1(-10, 480, &enemyStateData);
  /* EnemyStateData enemyStateData2; */
  /* initEnemy1(800, 580, &enemyStateData2); */
      EnemyBatData enemyBatData;
    initEnemyBat(&enemyBatData, 50, 360, 800);
      EnemyBatData enemyBatData1;
    initEnemyBat(&enemyBatData1, 0, 460, 800);
    Node **graph = create_graph(map);
    Node *goal = &graph[2][123];
    Node *startA = &graph[8][1];
    /* print_node(goal); */
    /* print_node(startA); */
    /* Node *list1 = a_star(graph, map, goal, start); */
    Node *list = a_star(graph, map, goal, startA);
    /* enemyBatData.state = BAT_MOVING_LEFT; */
    /* print_node(&list[4]); */

    /* for (int i = 0; i<33; i++){ */
    /*     print_node(&list[i]); */
    /* } */
    /* printf("size of list: %d\n", len_nodes(list)); */
    /* print_node(goal); */
    /* print_list(list, 30); */
    /* printf("first element :%d\n", list[0]); */
    /* print_list(pathList, 10); */
    /* print_node(graph[2][5].parent); */
    /* print_node(path); */



  while (running) {

    Uint64 start = SDL_GetTicks();

    if (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_SPACE) {
          jump(perso, map);
        }
      }
    }

        perso->vx = 0;
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_A])
            perso->vx -= MOOVSPEED;
        if (state[SDL_SCANCODE_D])
        perso->vx += MOOVSPEED;

            
    updateCam(perso, map);

        if (drawBackground(renderer, bgTextures, 5, map)) {
            printf("Error drawing the background");
            exit(-1);
        }
        if (drawMap(renderer, map, "./asset/tileset/ground-1.png", tileTextures)) {
            printf("Error drawing the map");
            exit(-1);
        }
        // if (fightMovement(renderer, event, playerInFight)) {
        // 	printf("Error drawing the fight");
        // 	exit(-1);
        // }
        SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
        if (display_perso(renderer, perso, map)) {
            printf("Error drawing the perso");
            exit(-1);
        }
        /* enemy1_movement(renderer, &enemyStateData, map); */
        /* enemy1Attack(&enemyStateData, perso, map); */
        updatePerso(perso, map, &enemyStateData);
        /* enemyBat_mouvement2(renderer, &enemyBatData, &list, map); */
        /* updatePerso(perso, map, &enemyStateData2); */
        /* renderStatusHealth(renderer, perso); */
                /* enemyBat_mouvement(renderer, &enemyBatData, map); */

        /* for (int i = 0; i<27; i++){ */
        /*     move_enemy_to_node(renderer, &enemyBatData, &list[i], map); */
        /* } */
        /* move_enemy_smoothly(renderer, &enemyBatData, &list, 27, 1, map); */
        /* printf("%d\n", list[0].x); */

        int goal_y = round(perso->y);
        int goal_x = round(perso->x);
        int start_y = round(enemyBatData.dst_rect.y/map->pix_rect);
        int start_x = round(enemyBatData.dst_rect.x/map->pix_rect);
        Node goalEnemy = graph[goal_y-1][goal_x-1];
        Node startEnemy = graph[start_y-1][start_x-1];
        /* Node *listB = a_star(graph, map, &goalEnemy, startA); */
        /* printf("start_x, start_y %d, %d\n", start_x, start_y); */
        /* printf("enemy: start_x, start_y %d, %d\n", enemyBatData.dst_rect.x, enemyBatData.dst_rect.y); */
        /* printf("goalEnemy (%d, %d)\n", goalEnemy.x, goalEnemy.y); */
        /* printf("walkable:%b\n", goalEnemy.walkable); */
        /* print_node(&graph[10][124]); */
        /* printf("path %d, %d, walkable:%b \n", graph[10][124].walkable, graph[10][124].x, graph[10][124].y ); */

        /* printf("start_x, start_y %f, %f\n", perso->x, perso->y); */
        /* Node *list = a_star(graph, map, goal, start); */
        /* follow_path(renderer, &enemyBatData, listB, map); */
        /* follow_path(renderer, &enemyBatData1, list, map); */
        /* printf("%d\n", enemyBatData.state); */
    
        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetTicks();
        float elapsedMS = (end - start);
        SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
        atexit(SDL_Quit);
    }
  /* print_graph(graph, map); */

  /* free(list); */
  free(graph);
    quitSDL(&renderer, &window, perso, map, playerInFight);
    atexit(SDL_Quit);
    return 0;
}
