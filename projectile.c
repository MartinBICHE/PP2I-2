#include "projectile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "perso.h"
#include "const.h"

bool isValidPosition(Map* map, float x, float y) {
    int tileX = (int)(x / map->pix_rect);
    int tileY = (int)(y / map->pix_rect);
    
    if (tileX >= 0 && tileX < map->width && tileY >= 0 && tileY < map->height) {
        char tile = map->matrix[tileY][tileX];
        if (tile == '-') {
            return true;
        }
    }
    return false;
}

void updateProjectile(Projectile* projectiles,Perso* perso, float targetX, float targetY, Map* map) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active == true) {
            float dx = (targetX-projectiles[i].x);
            float dy = (targetY-projectiles[i].y);
            float distance = sqrt(dx * dx + dy * dy); 
            projectiles[i].vx = (dx / distance) * PROJECTILE_SPEED;
            projectiles[i].vy = (dy / distance) * PROJECTILE_SPEED;
            projectiles[i].x += projectiles[i].vx*DT;
            projectiles[i].y += projectiles[i].vy*DT;
            // Mise à jour de la hitbox
            projectiles[i].hitbox.x = (int)projectiles[i].x;
            projectiles[i].hitbox.y = (int)projectiles[i].y;
            if (!isValidPosition(map, projectiles[i].x,projectiles[i].y) || checkProjectileCollisionWithPerso(&projectiles[i], perso)) {
                projectiles[i].active = false;
            }
        }
    }
}

void renderProjectiles(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            renderProjectile(projectiles[i].x,projectiles[i].y, renderer);
        }
    }
}

SDL_Texture* loadProjectileTexture(SDL_Renderer* renderer) {
    SDL_Surface* projectileSurface = IMG_Load("./asset/UI/projectile.png");
    if (!projectileSurface) {
        SDL_Log("Erreur lors du chargement de l'image du projectile : %s", IMG_GetError());
        return NULL;
    }

    SDL_Texture* projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
    if (!projectileTexture) {
        SDL_Log("Erreur lors de la création de la texture du projectile : %s", SDL_GetError());
        SDL_FreeSurface(projectileSurface); 
        return NULL;
    }

    SDL_FreeSurface(projectileSurface);

    return projectileTexture;
}

void renderProjectile(float x, float y, SDL_Renderer* renderer) {
    static SDL_Texture* projectileTexture = NULL;
    if (!projectileTexture) {
        projectileTexture = loadProjectileTexture(renderer);
        if (!projectileTexture) {
            return;
        }
    }

    SDL_Rect projectileRect = {.x = (int)x, .y = (int)y, .w = PROJECTILE_WIDTH, .h = PROJECTILE_HEIGHT};
    SDL_RenderCopy(renderer, projectileTexture, NULL, &projectileRect);
}


void spawnProjectile(int indice, int startTileX, int startTileY, float targetX, float targetY, Map* map) {
    if (indice < MAX_PROJECTILES) {
        projectiles[indice].x = startTileX * map->pix_rect;
        projectiles[indice].y = startTileY * map->pix_rect;
        // Initialisation de la hitbox
        projectiles[indice].hitbox.x = (int)projectiles[indice].x;
        projectiles[indice].hitbox.y = (int)projectiles[indice].y;
        projectiles[indice].hitbox.w = PROJECTILE_WIDTH;
        projectiles[indice].hitbox.h = PROJECTILE_HEIGHT;
        float dx = targetX - startTileX * map->pix_rect;
        float dy = targetY - startTileY * map->pix_rect;
        float distance = sqrt(dx * dx + dy * dy); 
        projectiles[indice].vx = (dx / distance) * PROJECTILE_SPEED;
        projectiles[indice].vy = (dy / distance) * PROJECTILE_SPEED;
        
        projectiles[indice].active = true;
    }
}

void cleanupProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            for (int j = i; j < MAX_PROJECTILES - 1; j++) {
                projectiles[j] = projectiles[j + 1];
            }
        }
    }
}

void printProjectiles(Projectile* projectiles) {
    printf("Projectile array:\n");
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        printf("Projectile %d - Position: (%f, %f), Velocity: (%f, %f), Active: %s\n", 
               i + 1, 
               projectiles[i].x, 
               projectiles[i].y, 
               projectiles[i].vx, 
               projectiles[i].vy, 
               projectiles[i].active ? "true" : "false");
    }
}

bool checkProjectileCollisionWithPerso(Projectile* projectile, Perso* perso) {
    SDL_Rect projectileRect = projectile->hitbox;
    SDL_Rect persoRect = perso->hitbox;
    return SDL_HasIntersection(&projectileRect, &persoRect);
}