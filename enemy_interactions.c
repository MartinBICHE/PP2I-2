#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "enemy_interactions.h"
#include "const.h"
#include "enemy2.h"

#define ATTACK_RANGE 50 // en pixels, valeur arbitraire pour l'instant
#define ATTACK_DAMAGE 10 // valeur arbitraire pour l'instant

// Fonction pour vérifier si le personnage est proche d'un ennemi
int isCharacterNearEnemy(Perso *perso, Enemy2 *enemy) {
    float distance = sqrt(pow(enemy->xPosition - perso->x, 2) + pow(enemy->yPosition - perso->y, 2));
    return (distance < ATTACK_RANGE);
}

// Fonction pour gérer l'attaque du personnage
void handleCharacterAttack(Perso *perso, Enemy2 *enemy) {
    if (isCharacterNearEnemy(perso, enemy)) {
        // Diminuer la santé de l'ennemi
        enemy->health -= ATTACK_DAMAGE;
    }

// Fonction pour vérifier la collision entre le personnage et un ennemi
bool checkCollision(Perso *perso, Enemy2 *enemy) {
    SDL_Rect persoRect = perso->hitbox;
    SDL_Rect enemyRect = {.x = enemy->xPosition, .y = enemy->yPosition, .w = ENEMY_WIDTH, .h = ENEMY_HEIGHT}; // Il faudrait pouvoir accéder à la largeur et à la hauteur de l'ennemi
    // Vérifier si les deux rectangles se chevauchent
    return SDL_HasIntersection(&persoRect, &enemyRect);
}

}

