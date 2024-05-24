#include "main.h"

SDL_Texture *bgTextures[6];
SDL_Texture *tileTextures, *persoTexture;



int main(int argc, char **argv) {

    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(&window, &renderer);

    // const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};

    Perso *playerInFight = (Perso *)malloc(sizeof(Perso));
    playerInFight->y = QUARTERHEIGHT - SPRITESIZE / 2;
    playerInFight->x = TIERWIDTH / 2 - SPRITESIZE / 2;
    Map *map = initMap("map2");
    Perso *perso = create_perso(map);

    CheckpointList *checkpointList = malloc(sizeof(CheckpointList));
    initCheckpointList(checkpointList);

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    SDL_Event event;
    int running = 1;


    loadBackgroundTextures(renderer, bgTextures, 5);
    loadTileTextures(renderer, &tileTextures, "./asset/tileset/ground-1.png");

    loadPersoTexture(renderer, &persoTexture, "./asset/spritesheet/ss_mc.png");

    EnemyStateData enemyStateData;
    initEnemy1(600, 660, &enemyStateData);

     Node **graph = create_graph(map);
     Node *goal = &graph[13][134];
    Node *startA = &graph[10][2];
    Node *list = a_star(graph, map, goal, startA);
    /* loadPosition("temp.sav", perso); */
    Enemy2 enemy;
    initEnemy2(&enemy, startA, goal, map);

    EnemyBatData enemyBatData;
    initEnemyBat(&enemyBatData, 100, 0,  1000, goal, startA, map);

    while (running) {

        Uint64 start = SDL_GetTicks();

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            }
        }
        
        updatePerso(perso, map, &enemyStateData, state);
        updateCam(perso, map);

        if (drawBackground(renderer, bgTextures, 5, map)) {
            printf("Error drawing the background");
            exit(-1);
        }
        if (drawMap(renderer, map, tileTextures)) {
            printf("Error drawing the map");
            exit(-1);
        }
        // if (fightMovement(renderer, event, playerInFight)) {
        // 	printf("Error drawing the fight");
        // 	exit(-1);
        // }
        if (display_perso(renderer, perso, map, persoTexture, 0)) { // 1 pour afficher la hitbox
            printf("Error drawing the perso");
            exit(-1);
        }

        
        /* int goal_y = round(perso->y); */
        /* int goal_x = round(perso->x); */
        /* Node goalEnemy = graph[goal_y-1][goal_x-1]; */
        /* Node *listB = a_star(graph, map, &goalEnemy, startA); */

        /* follow_path(renderer, &enemyBatData, list, map, perso); */
        /* enemy1_movement(renderer, &enemyStateData, map); */
        /* enemy1Attack(&enemyStateData, perso, map); */
        /* enemyBat_mouvement(renderer, &enemyBatData, map, perso); */
        /* batAttack(&enemyBatData, perso, map); */
        renderStatusHealth(renderer, perso);
        /* follow_path2(renderer, &enemyBatData, graph, map, perso); */
         handleDeath(perso, "temp.sav");
         /* enemy2_follow(renderer, &enemy, graph, map); */
         /* enemy2Attack(&enemy, perso, map); */
         enemyBat_follow(renderer, &enemyBatData, graph, map, perso);
        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetTicks();
        float elapsedMS = (end - start);
        SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
        }


    free(list);
    free_graph(graph, map);
    free_checkpoints(checkpointList);
    /* à mettre dans le init après */
    quitSDL(&renderer, &window, perso, map, playerInFight);
    atexit(SDL_Quit);
    return 0;
}
