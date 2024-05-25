#include <SDL2/SDL_mixer.h>
#include "music.h"

Mix_Chunk *musicEnemyFleche = NULL;
Mix_Chunk *musicEnemySnake = NULL;
Mix_Chunk *musicEnemyBat = NULL;
Mix_Chunk *musicEnemyBatAttack = NULL;

int load_music() {
    musicEnemyFleche = Mix_LoadWAV("asset/Music/fleche.wav");
    musicEnemySnake = Mix_LoadWAV("asset/Music/snake.wav");
    musicEnemyBat = Mix_LoadWAV("asset/Music/batWings.wav");
    musicEnemyBatAttack = Mix_LoadWAV("asset/Music/batAttack.wav");
    if (musicEnemyFleche == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    if (musicEnemySnake == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    if (musicEnemyBat == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    if (musicEnemyBatAttack == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 1;
    }
    return 0;
}


int free_music(){
    Mix_FreeChunk(musicEnemyFleche);
    Mix_FreeChunk(musicEnemySnake);
    Mix_FreeChunk(musicEnemyBat);
    Mix_FreeChunk(musicEnemyBatAttack);
    return 0;
}

