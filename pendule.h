#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define PI 3.14159


struct _PenduleData{
    float time;
    float x;
    float y;
    int textureWidth;
    int textureHeight;
    int scale;
    float period;
    float amplitude;
};
typedef struct _PenduleData PenduleData;

void initPendule(PenduleData *penduleData, SDL_Texture *texture);
void pendule_mouvement(SDL_Renderer *renderer, SDL_Texture *texture, int xInit, int yInit, PenduleData *penduleData);
