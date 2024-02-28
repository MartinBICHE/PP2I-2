#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]){
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Jeux xd",    /* nom de la fênetre */
            SDL_WINDOWPOS_UNDEFINED,        /* position x dans la fênetre */ 
            SDL_WINDOWPOS_UNDEFINED,        /* position y dans la fênetre */ 
            840,                            /*largeur de la fênetre */
            680,                            /*hauteur de la fênetre */
            0);                             /*de flags, ex: SDL_WINDOW_FULLSCREEN, 0 sinon */

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);      /* couleurs en rbg(0, 0, 0) puis l'opacité: 255 */
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Event event;
    int quit = 0;
    while (!quit){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){   /* idk ce que ça fait actually */
                quit = 1;
            }
            if (event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:           /*on quitte la fênetre lorsqu'on appuie sur esc*/
                        quit = 1;
                        SDL_DestroyWindow(window);
                        break;
                }
            }
            if (event.type == SDL_WINDOWEVENT_CLOSE){   /*idem ici lorsqu'on ferme la fênetre */
                quit = 1;
                SDL_DestroyWindow(window);
                break;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
