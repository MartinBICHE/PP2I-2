#include "main.h"
#include "checkpoints.h"
#include "const.h"
#include "dialog_box.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "init.h"
#include "map.h"
#include "perso.h"
#include "scroll.h"
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
#include "pendule.h"
#include "textures.h"
#include "fonts.h"

int distance = 0;
SDL_Texture *bgTextures[6];

int main(int argc, char **argv) {
  int b = 1;

  SDL_Window *window;
  SDL_Renderer *renderer;
  initSDL(&window, &renderer);

  TTF_Font *font1 = TTF_OpenFont("asset/Fonts/DisposableDroidBB.ttf", 20);
  TTF_Font *font2 = TTF_OpenFont("asset/Fonts/DisposableDroidBB_bld.ttf", 20);
  TTF_Font *font3 = TTF_OpenFont("asset/Fonts/ChopinScript.ttf", 23);
  SDL_Rect dst_rect1 = {100, 480, 64, 64};
  SDL_Rect src_rect1 = {0, 0, 64, 64};
  SDL_Rect dst_rect2 = {160, 300, 64, 64};
  SDL_Rect src_rect2 = {0, 0, 64, 64};

  SDL_Surface *surfaceScroll = IMG_Load("asset/spritesheet/scroll.png");
  SDL_Surface *surfacePapirusA = IMG_Load("asset/spritesheet/papirus.png");
  SDL_Surface *surfaceEnnemy3 = IMG_Load("ennemy3.png");
  SDL_Surface *surfacePendule = IMG_Load("pendule.png");

  /* SDL_Texture *textureScroll = */
  /*     SDL_CreateTextureFromSurface(renderer, surfaceScroll); */
  if (textureScroll == NULL){
      puts("lksdfjslfkjslfkjflkfjslfjlmkjfsl");
  }
  SDL_Texture *texturePapirusA =
      SDL_CreateTextureFromSurface(renderer, surfacePapirusA);
  /* SDL_Texture *texturePendule = */
  /*     SDL_CreateTextureFromSurface(renderer, surfacePendule); */
  SDL_Texture *textureEnnemy3 = SDL_CreateTextureFromSurface(renderer, surfaceEnnemy3);
  SDL_Rect dst_rectScroll = {460, 280, 250, 10};
  SDL_Rect src_rectScroll = {0, 0, 250, 10};
  SDL_Rect dst_rectScroll2 = {30, 280, 250, 10};
  SDL_Rect src_rectScroll2 = {0, 0, 250, 10};
  const char *text =
      "A fost odata ca niciodata o baba si un mosneag care au murit de mititei";
  const char *text2 =
      "hi my name is ";
  SDL_Rect dest_rect = {100, 460, 64, 64};
  SDL_Rect src_rect = {0, 128, 64, 64};
  SDL_Rect dest_rectB = {190, 460, 64, 64};
  SDL_Rect src_rectB = {0, 128, 64, 64};
  SDL_Rect dest_rectC = {250, 460, 64, 64};
  SDL_Rect src_rectC = {0, 128, 64, 64};
  /* enum EnemyState enemyState; */
  /* EnemyStateData *enemyStateData; */
  EnemyStateData enemyState;
  enemyState.state = MOVING_UP;
  enemyState.pauseStartBits = 0;
  enemyState.pauseStart = 0;

  EnemyStateData enemyState1;
  enemyState1.state = MOVING_UP;
  enemyState1.pauseStartBits = 0;
  enemyState1.pauseStart = 0;

  SDL_Rect dst_rectT;
  SDL_Rect src_rectT;
  EnemyStateData enemyState2;
  SDL_Rect dst_rectR;
  SDL_Rect src_rectR;
  EnemyStateData enemyState3;
  initEnemy1(100, 460, &enemyState2);
  initEnemy1(180, 460, &enemyState3);


  /* initPapirus(&dst_rectR, 100, 100); */

  ScrollStateData scrollStateDatar;
  initScroll(&scrollStateDatar, 100, 100);

  ScrollStateData scrollStateDataA;
  initScroll(&scrollStateDataA, 200, 200);


  Enemy3 enemy3;
  enemy3.xPosition = dst_rect1.x;
  enemy3.yPosition = dst_rect1.y;
  enemy3.speed = 10;
  enemy3.xCollisionMax = 400;
  enemy3.xCollisionMin = dst_rect1.x;
  enemy3.dx = 1;

  Enemy3 enemy3Bis;
  enemy3Bis.xPosition = 100;
  enemy3Bis.yPosition = 460;
  enemy3Bis.speed = 10;
  enemy3Bis.xCollisionMax = 500;
  enemy3Bis.xCollisionMin = 100;
  enemy3Bis.dx = 1;


  SDL_Rect dst_rect5;
  SDL_Rect src_rect5;
  Enemy3 enemy35;
  initEnemy3(&enemy35, 100, 460, 500);

  SDL_Rect dst_rect6;
  SDL_Rect src_rect6;
  Enemy3 enemy36;
  initEnemy3(&enemy36, 300, 260, 600);


  ScrollStateData scrollStateData;
  scrollStateData.pauseStart = 0;
  scrollStateData.currentCharIndex = 0;
  scrollStateData.delayTimer = 0;


  PenduleData penduleData;
  penduleData.time = 0;

  int i = 0;
  SDL_Texture *textureF;
  SDL_Texture *textureF2;
  SDL_Surface *SurfaceF = IMG_Load("ennemy1.png");
  SDL_Surface *SurfaceF2 = IMG_Load("ennemy1.png");
  /* SDL_Rect *dst_rectF = malloc(sizeof(SDL_Rect)); */
  /* SDL_Rect *src_rectF = malloc(sizeof(SDL_Rect)); */
  /* SDL_Rect *dst_rectF2 = malloc(sizeof(SDL_Rect)); */
  /* SDL_Rect *src_rectF2 = malloc(sizeof(SDL_Rect)); */
  /* initEnemy1(renderer, &textureF2, SurfaceF2, src_rectF2, dst_rectF2, 160,
   * 460); */
  /* initEnemy1(renderer, &textureF, SurfaceF, src_rectF, dst_rectF, 105, 460);
   */

  Perso *perso3 = malloc(sizeof(Perso));
  /* perso3->x = 45; */
  /* perso3->y = 3; */
  /* perso3->vx = 0; */
  /* perso3->vy = 0; */
  /* savePosition("temp.sav", perso3); */
  CheckpointList *checkpointList = malloc(sizeof(CheckpointList));
  initCheckpointList(checkpointList);
  addCheckpoint(checkpointList, 46);
  addCheckpoint(checkpointList, 30);
  addCheckpoint(checkpointList, 20);
  addCheckpoint(checkpointList, 10);

  const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};
  // const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
  const SDL_Color RED = {.r = 255, .g = 0, .b = 0, .a = 0};


  float amplitude = -1*PI/2.0;
  float period = 4;
  float timeStep = 0.05;
  float time = 0.0;
  int x_pen = 250;
  int y_pen = 250;


  PenduleData penduleDataI;
  initPendule(&penduleDataI, texturePendule);
  PenduleData penduleDataR;
  initPendule(&penduleDataR, texturePendule);


  DialogBoxData dialogBoxData;
  initPapirus(&dialogBoxData, 100, 100);

  Map *map = initMap("map1/data.txt");
  Perso *perso = create_perso(map);
  /* loadPosition("temp.sav", perso); */
  int c = 1;
  int d = 1;

  float x_cam = 0; // cam à gauche au début

  SDL_Event event;
  int running = 1;

  loadBackgroundTextures(renderer, bgTextures, 5);

  /* loadFonts(); */
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

    updatePerso(perso, map);
    x_cam = updateCam(perso->x * PIX_RECT, x_cam);
    checkCheckpoint(checkpointList, perso, "temp.sav");

    if (drawBackground(renderer, bgTextures, 5, x_cam)) {
      printf("Error drawing the background");
      exit(-1);
    }
    if (drawMap(renderer, map, "./asset/tileset/ground-1.png", x_cam)) {
      printf("Error drawing the map");
      exit(-1);
    }

    SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
    if (display_perso(renderer, perso, x_cam)) {
      printf("Error drawing the perso");
      exit(-1);
    }

    // SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a); //
    // !!! seulement pour les tests de caméra (à changer) SDL_Rect rect1 = {.x =
    // x_perso*PIX_RECT - 10 - x_cam, .y = 3*PIX_RECT - 10, .w = 20, .h = 20};
    // // !!! seulement pour les tests de caméra (à changer)
    // SDL_RenderDrawRect(renderer, &rect1); // !!! seulement pour les tests de
    // caméra (à changer) SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g,
    // BLACK.b, BLACK.a); // !!! seulement pour les tests de caméra (à changer)
    // SDL_Rect rect2 = {.x = x_perso*PIX_RECT - 9 - x_cam, .y = 3*PIX_RECT - 9,
    // .w = 18, .h = 18}; // !!! seulement pour les tests de caméra (à changer)
    // SDL_RenderDrawRect(renderer, &rect2); // !!! seulement pour les tests de
    // caméra (à changer)


    /* enemy1_movement(renderer, textureEnemy1, &enemyState2); */
    /* enemy1_movement(renderer, textureEnnemy2, &enemyState3); */
    /* enemy1_movement_4(renderer, textureEnnemy2, &src_rectT, &dst_rectT, &enemyState3); */
    /* enemy3_movement(renderer, textureEnnemy3, &src_rect1, &dst_rect1, &enemy3); */

    /* enemy3_movement(renderer, textureEnemy3, &enemy35); */
    /* enemy3_movement(renderer, textureEnnemy3, &enemy36); */
    /* enemy3_movement(renderer, textureEnnemy3, &src_rect6, &dst_rect6, &enemy36); */


    /* render_text(renderer, font1, text, BLACK, &dst_rect1, texturePapirus, font2); */
    /* render_text_2(renderer, font1, text, 100, 100, BLACK, &dst_rect1, texturePapirus, font2); */
    /* render_text(renderer, fontDialogBox, text, BLACK, &dialogBoxData, texturePapirus, fontDialogBoxBold); */
    /* render_text_2(renderer, font1,text, 200, 200, BLACK, &dst_rectR, texturePapirus, font2); */

    /* scroll_movement(renderer, textureScroll, &src_rectScroll,  &dst_rectScroll, fontScroll, text2, BLACK); */
    /* scroll_movement_2(renderer, textureScroll, font3, text, BLACK, &scrollStateDatar); */
    /* scroll_movement_2(renderer, textureScroll, font3, text2, BLACK, &scrollStateDataA); */
    /* render_sprite_2(renderer, x_pen+160, y_pen+10, 3, "bubble.png"); */
    /* render_sprite_2(renderer, x_pen+260, y_pen+90, 3, "bubble.png"); */
    /* pendule_mouvement(renderer, textureScroll, amplitude, period, x_pen, y_pen, &penduleData ); */
    /* pendule_mouvement(renderer, texturePendule, 100, 100, &penduleDataI); */
    /* pendule_mouvement(renderer, texturePendule, 400, 400, &penduleDataR); */

    SDL_RenderPresent(renderer);

    Uint64 end = SDL_GetTicks();
    float elapsedMS = (end - start);
    SDL_Delay(fmaxf((1000 * DT - elapsedMS) / 1.0f, 0));
  }

  TTF_CloseFont(font1);
  TTF_CloseFont(font2);
  TTF_CloseFont(font3);
  quitSDL(&renderer, &window, perso, map);
  atexit(SDL_Quit);
  free(checkpointList->xPositions);
  free(checkpointList);
  return 0;
}
