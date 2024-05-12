#include "projectile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


bool isValidPosition(Map* map, float x, float y) {
    // Convertir les coordonnées en indices de matrice
    int tileX = (int)(x / map->pix_rect);
    int tileY = (int)(y / map->pix_rect);
    
    // Vérifier si les indices sont à l'intérieur des limites de la matrice
    if (tileX >= 0 && tileX < map->width && tileY >= 0 && tileY < map->height) {
        // Récupérer le caractère à ces indices dans la matrice
        char tile = map->matrix[tileY][tileX];
        // Vérifier si le caractère est "-"
        if (tile == '-') {
            // Les coordonnées sont sur une case valide
            return true;
        }
    }
    
    // Les coordonnées ne sont pas sur une case valide
    return false;
}

void updateProjectile(Projectile* projectiles, float targetX, float targetY, Map* map) {
    // Mettre à jour et vérifier chaque projectile
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active == true) {
            // Mettre à jour la position du projectile en fonction de sa vitesse
            float dx = (targetX-projectiles[i].x);
            float dy = (targetY-projectiles[i].y);
            float distance = sqrt(dx * dx + dy * dy); // Calcul de la distance entre les points
            projectiles[i].vx = (dx / distance) * PROJECTILE_SPEED;
            projectiles[i].vy = (dy / distance) * PROJECTILE_SPEED;
            projectiles[i].x += projectiles[i].vx*DT;
            projectiles[i].y += projectiles[i].vy*DT;

            // Vérifier si la nouvelle position du projectile est valide sur la carte
            if (!isValidPosition(map, projectiles[i].x,projectiles[i].y)) {
                // La position est invalide, donc le projectile a rencontré un obstacle
                projectiles[i].active = false;
            }
        }
    }
}

void renderProjectiles(SDL_Renderer* renderer) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            // Convertir les coordonnées absolues en indices de tuiles
            renderProjectile(projectiles[i].x,projectiles[i].y, renderer);
        }
    }
}

// Fonction pour charger la texture du projectile
SDL_Texture* loadProjectileTexture(SDL_Renderer* renderer) {
    // Chargez l'image du projectile depuis le fichier PNG
    SDL_Surface* projectileSurface = IMG_Load("./asset/UI/projectile.png");
    if (!projectileSurface) {
        // Gestion de l'erreur de chargement de l'image
        SDL_Log("Erreur lors du chargement de l'image du projectile : %s", IMG_GetError());
        return NULL;
    }

    // Convertir la surface en texture
    SDL_Texture* projectileTexture = SDL_CreateTextureFromSurface(renderer, projectileSurface);
    if (!projectileTexture) {
        // Gestion de l'erreur de création de la texture
        SDL_Log("Erreur lors de la création de la texture du projectile : %s", SDL_GetError());
        SDL_FreeSurface(projectileSurface); // Libérer la surface pour éviter les fuites de mémoire
        return NULL;
    }

    // Libérer la surface car nous n'en avons plus besoin
    SDL_FreeSurface(projectileSurface);

    return projectileTexture;
}

// Fonction pour rendre le projectile à l'écran
void renderProjectile(float x, float y, SDL_Renderer* renderer) {
    // Chargez la texture du projectile au démarrage du jeu
    static SDL_Texture* projectileTexture = NULL;
    if (!projectileTexture) {
        projectileTexture = loadProjectileTexture(renderer);
        if (!projectileTexture) {
            // Gestion de l'erreur de chargement de la texture
            return;
        }
    }

    // Rendu du projectile à l'écran
    SDL_Rect projectileRect = {.x = (int)x, .y = (int)y, .w = PROJECTILE_WIDTH, .h = PROJECTILE_HEIGHT};
    SDL_RenderCopy(renderer, projectileTexture, NULL, &projectileRect);
}


void spawnProjectile(int indice, int startTileX, int startTileY, float targetX, float targetY, Map* map) {
    if (indice < MAX_PROJECTILES) {
        // Créer un nouveau projectile et l'ajouter à la liste
        projectiles[indice].x = startTileX * map->pix_rect;
        projectiles[indice].y = startTileY * map->pix_rect;
        // Calculer la direction du projectile (vecteur unitaire)
        float dx = targetX - startTileX * map->pix_rect;
        float dy = targetY - startTileY * map->pix_rect;
        float distance = sqrt(dx * dx + dy * dy); // Calcul de la distance entre les points
        projectiles[indice].vx = (dx / distance) * PROJECTILE_SPEED;
        projectiles[indice].vy = (dy / distance) * PROJECTILE_SPEED;
        
        projectiles[indice].active = true;
    }
}

void cleanupProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            // Supprimer le projectile de la liste
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