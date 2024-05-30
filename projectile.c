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

void updateProjectile(Projectile* projectiles, Perso* perso, float targetX, float targetY, Map* map,Boss* boss) {
    for (int i = 0; i < MAX_PROJECTILES ; i++) {
        if (projectiles[i].active) {
            float dx = targetX - projectiles[i].x;
            float dy = targetY - projectiles[i].y;
            float distance = sqrtf(dx * dx + dy * dy);

            // Direction actuelle du projectile
            float currentDirX = projectiles[i].vx;
            float currentDirY = projectiles[i].vy;
            float currentDirLength = sqrtf(currentDirX * currentDirX + currentDirY * currentDirY);
            if (currentDirLength != 0) {
                currentDirX /= currentDirLength;
                currentDirY /= currentDirLength;
            }

            // Direction vers le personnage
            float desiredDirX = dx / distance;
            float desiredDirY = dy / distance;

            // Calculer l'angle entre les deux directions
            float dotProduct = currentDirX * desiredDirX + currentDirY * desiredDirY; // Calcul du produit scalaire 
            float angle = acosf(dotProduct);
            float crossProduct = currentDirX * desiredDirY - currentDirY * desiredDirX; // Calcul du produit vectoriel
            float sign = (crossProduct < 0) ? -1 : 1; // Détermination du signe de l'angle 

            // Limiter l'angle de rotation
            float rotationAngle = fminf(angle, MAX_ROTATION_ANGLE);
            float cosAngle = cosf(rotationAngle * sign);
            float sinAngle = sinf(rotationAngle * sign);

            // Appliquer la rotation limitée à la direction actuelle
            float newDirX = currentDirX * cosAngle - currentDirY * sinAngle;
            float newDirY = currentDirX * sinAngle + currentDirY * cosAngle;

            // Mettre à jour la vitesse du projectile
            projectiles[i].vx = newDirX * projectileSpeed;
            projectiles[i].vy = newDirY * projectileSpeed;
            projectiles[i].x += projectiles[i].vx * DT;
            projectiles[i].y += projectiles[i].vy * DT;

            // Mise à jour de la hitbox
            projectiles[i].hitbox.x = (int)projectiles[i].x;
            projectiles[i].hitbox.y = (int)projectiles[i].y;

            // Vérifier la collision avec les tuiles
            checkProjectileCollisionWithTiles(&projectiles[i], map);

            // Vérifier la collision avec le personnage ou les limites de la carte
            if (!isValidPosition(map, projectiles[i].x, projectiles[i].y) || checkProjectileCollisionWithPerso(&projectiles[i], perso)) {
                // Si le projectile touche le personnage, appliquer le recul
                if (checkProjectileCollisionWithPerso(&projectiles[i], perso)) {
                    perso -> health--;
                    // Calculer la direction du recul
                    float recoil_dx = projectiles[i].vx;
                    float recoil_dy = projectiles[i].vy;
                    float recoil_distance = sqrt(recoil_dx * recoil_dx + recoil_dy * recoil_dy);
                    // Normaliser la direction du recul
                    recoil_dx /= recoil_distance;
                    recoil_dy /= recoil_distance;
                    // Appliquer le recul au personnage
                    float recoil_strength = 12.0f; // Augmentez cette valeur pour intensifier le recul
                    perso->vx += recoil_dx * recoil_strength;
                    perso->vy += recoil_dy * recoil_strength;
                    // Activer le mode recul
                    perso->recoil_timer = 8; // Définir la durée du recul en frames
                }
                // if (checkProjectileCollisionWithBoss(&projectiles[i], boss)){
                //     boss -> health--;
                // }
                projectiles[i].active = false;
            }
            
            for (int j = 0; j < MAX_PROJECTILES ; j++) {
                if (i != j && projectiles[j].active && SDL_HasIntersection(&projectiles[i].hitbox, &projectiles[j].hitbox)) {
                    projectiles[i].active = false;
                    projectiles[j].active = false;
                    break;
                }
            }
        }
    }
}

int renderProjectile(SDL_Renderer* renderer, Projectile *projectile, Map *map, SDL_Texture *projectileTexture, int showHitbox) {
    int c = 50;

    // Définir la destination du rectangle
    SDL_Rect dst_rect = {.x = (int)projectile->x, .y = (int)projectile->y, .w = c, .h = c};

    // Mettre à jour l'offset du sprite toutes les 4 frames
    if (!afficherImage && !parametre) {
        projectile->spriteOffset = (projectile->spriteOffset + 1) % 24; // 24 est le cycle total (6 sprites * 4 frames par sprite)
    }

    // Calculer l'index du sprite actuel (chaque sprite reste pendant 4 frames)
    int currentSprite = (projectile->spriteOffset / 4) % 6;

    // Définir le rectangle source basé sur l'index du sprite
    SDL_Rect src_rect = {.x = currentSprite * 16, .y = 0, .w = 16, .h = 16};

    // Rendre le sprite de projectile
    if (SDL_RenderCopy(renderer, projectileTexture, &src_rect, &dst_rect) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_RenderCopy error: %s", SDL_GetError());
    }

    // Afficher la hitbox si demandé
    if (showHitbox) {
        if (display_projectile_hitbox(renderer, projectile, map)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in display projectile hitbox: %s", SDL_GetError());
            exit(-1);
        }
    }

    return 0;
}


int display_projectile_hitbox(SDL_Renderer *renderer, Projectile *projectile, Map *map) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect rect1 = {.x = projectile->hitbox.x, .y = projectile->hitbox.y, .w = projectile->hitbox.w, .h = projectile->hitbox.h};
    if (SDL_RenderDrawRect(renderer, &rect1)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in draw rect : %s", SDL_GetError());
		exit(-1);
    }
    return 0;
}

void renderProjectiles(SDL_Renderer* renderer, Map *map) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            renderProjectile(renderer, &projectiles[i], map, projectileTexture,0);
        }
    }
}

void loadProjectileTexture(SDL_Renderer* renderer, SDL_Texture **projectileTexture, char *path) {
    SDL_Surface* projectileSurface = IMG_Load(path);
    if (!projectileSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in projectile surface init %s", IMG_GetError());
        exit(-1);
    }

    *projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
    if (!*projectileTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in projectile texture init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_FreeSurface(projectileSurface);
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
        projectiles[indice].vx = (dx / distance) * projectileSpeed;
        projectiles[indice].vy = (dy / distance) * projectileSpeed;
        
        projectiles[indice].active = true;
    }
}

void cleanupProjectiles(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            for (int j = i; j < MAX_PROJECTILES - 1; j++) {
                projectiles[j] = projectiles[j + 1];
            }
        }
    }
}

void freeProjectileTexture(void) {
    if (projectileTexture) {
        SDL_DestroyTexture(projectileTexture);
        projectileTexture = NULL;
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

bool checkProjectileCollisionWithBoss(Projectile* projectile, Boss* boss) {
    SDL_Rect projectileRect = projectile->hitbox;
    SDL_Rect bossRect = boss->hitbox;
    return SDL_HasIntersection(&projectileRect, &bossRect);
}

void checkProjectileCollisionWithTiles(Projectile* projectile, Map* map) {
    int tileX = (int)(projectile->x / map->pix_rect);
    int tileY = (int)(projectile->y / map->pix_rect);

    if (tileX >= 0 && tileX < map->width && tileY >= 0 && tileY < map->height) {
        if (map->matrix[tileY][tileX] == 'm') {
            map->matrix[tileY][tileX] = '-'; // Remplacer 'z' par '-' pour indiquer que la tuile est cassée
            projectile->active = false; // Désactiver le projectile après avoir cassé la tuile
        }
    }
}

void resetProjectiles(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].x = 0.0f;
        projectiles[i].y = 0.0f;
        projectiles[i].vx = 0.0f;
        projectiles[i].vy = 0.0f;
        projectiles[i].active = false;
    }
}
