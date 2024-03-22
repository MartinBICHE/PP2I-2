#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "dialog_box.h" /*fichier qui contient des fonctions utiles tout au long du projet, ex: afficher un sprite, afficher tu texte, (c'est réutilisable car on peut fixer les paramètres) */



int main(){
    int box = 0; /*pour afficher la boite dialogue */
    int time = 0;   /*pour commencer le chrono & affichage du temps */
    int visible_text = 1; /* le texte press return etc... */
    int running = 1;  /*main loop */
    int const WIDTH = 840;   /*pour la fênetre */
    int const HEIGHT = 680;
    SDL_Color white_color = {255, 255, 255};
    SDL_Color black_color = {0, 0, 0};

   /* init SDL */
    if (SDL_Init(SDL_INIT_EVERYTHING)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
        exit(-1);
    }
    if (TTF_Init()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in font init: %s", TTF_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
    atexit(TTF_Quit);



    /* init fenêtre et renderer */
    SDL_Color const BACKGROUND_COLOR = {.r = 0xD0, .g = 0xD0, .b = 0xD0, .a = SDL_ALPHA_OPAQUE};
    SDL_Window *window;
    window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    TTF_Font *font = TTF_OpenFont("EBGaramond-Regular.ttf", 24);

    if (!window){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); /*pour éviter le rédimensionement des images lors de changement de résolution, et tout cas je l'espère */


    SDL_Event event;

    while (running){
        if (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_RETURN){
                       SDL_RenderClear(renderer);
                       visible_text = 0;
                       box = 1;
                       time = 1;

                    }
                    // frappes claviers
                    break;
                // autres événements
                default:
                        break;
            }
             /* action des mobs & gestion des interactions */
             /* changements des états du jeu */


           }


             /* rendu graphique */ 
           if (SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a)){
               SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render draw color: %s", SDL_GetError());
           }
           if (SDL_RenderClear(renderer)){
               SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in set render clear: %s", SDL_GetError());
           }

           /* game elements */

           if (box){
               SDL_RenderClear(renderer);
               render_sprite(renderer, 420, 250, 40, "noir.png");
               print_string("Good morning, good morning", font, white_color, renderer, 430, 550, 1);
               /* const char *text_del1 = "Phrase lkksdj jslfkdsl kfjdljkls "; */
               /* text_in_box(text_del1, font, white_color, renderer, 430, 550, 1, 810); */
           }
           if (time){
               Uint32 start_time = SDL_GetTicks();
               char *time = show_time(start_time);
               char *chrono = "Petit chrono";
               print_string(chrono, font, black_color, renderer, 10, 0, 1);
               print_string(time, font, black_color, renderer, 10, 20, 1);
           }
           const char *text = "Press Return to start";
           if (visible_text){
               print_string(text, font, black_color, renderer, 3, 0, 1);
           }


           /* render_sprite(renderer, 420, 250, 40, "noir.png"); */
           /* const char *text_del ="Ceci est un example de phrase longue pour voir si le texte s'affiche bien dans la boite"; */
           /* text_in_box(text_del, font, white_color, renderer, 430, 550, 1, 810); */
           SDL_RenderPresent(renderer);
    }

}


