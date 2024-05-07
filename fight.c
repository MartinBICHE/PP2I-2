#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "const.h"
#include "fight.h"






/* Fonction de gestion du personnage */

int fightMovement(SDL_Renderer *renderer, SDL_Event event, PersoFight *player) {
    static int offset = 0;
    static int line = 0;

    static SDL_Texture *spriteTexture = NULL;
    if (!spriteTexture) {
        SDL_Surface *spriteSurface = IMG_Load("./asset/spritesheet/ss_mc.png");
        if (!spriteSurface) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite surface init: %s", IMG_GetError());
            exit(-1);
        }
        spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        if (!spriteTexture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in sprite texture init: %s", SDL_GetError());
            exit(-1);
        }
        SDL_FreeSurface(spriteSurface);
    }

    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture,NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in query texture: %s", SDL_GetError());
        exit(-1);
    }

    int spriteHeight = spriteFullHeight / 4; 
    int spriteWidth = spriteFullWidth / 12;

    const int INPUT_DELAY_MS = 3; 
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    static clock_t lastInputTime = 0;
    clock_t currentTime = clock();

    if ((double)(currentTime - lastInputTime) / CLOCKS_PER_SEC * 1000 >= INPUT_DELAY_MS) {
        lastInputTime = currentTime;
        if (keyboardState[SDL_SCANCODE_UP]) {
            if (player->y >  QUARTERHEIGHT) {
                player->y -= 2 * QUARTERHEIGHT;
            }
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            if ( player->y < WINHEIGHT - 2 * QUARTERHEIGHT - 1.5*SPRITESIZE) {
                player->y += 2 * QUARTERHEIGHT;
            }
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            if (player->x > TIERWIDTH) {
                player->x -= TIERWIDTH;
            }
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            if ( player->x <= WINWIDTH-TIERWIDTH) {
                player->x += TIERWIDTH;
            }
        }
    }


    SDL_Rect spriteRect = {.x = offset * spriteWidth, .y = line * spriteHeight, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = { .x = player->x, .y = player->y, .w = 1.5*spriteWidth, .h = 1.5*spriteHeight};

    if (SDL_RenderCopyEx(renderer, spriteTexture, &spriteRect, &destRect, 0, NULL, SDL_FLIP_NONE)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error in render copy: %s", SDL_GetError());
        exit(-1);
    }
    SDL_RenderPresent(renderer);

    return 0;
}

void takeDamage(AttackFight *attack, PersoFight *player) {
    if (player->x <= attack->x + TIERWIDTH && player->x >= attack->x && player->y <= attack->y + QUARTERHEIGHT && player->y >= attack->y && player->iframe == 0) {
        player->health -= 1;
        player->iframe = 200;
    }
    if (player->health == 0) {
        printf("You died\n");
        exit(0);
    }
}

void invincibility(PersoFight *player) {
    if (player->iframe > 0) {
        player->iframe -= 1;
    }
}

void damageBoss(AttackFight *hitPoint, PersoFight *player, bossFight *boss) {
    if (player->x <= hitPoint->x + TIERWIDTH && player->x >= hitPoint->x && player->y <= hitPoint->y + QUARTERHEIGHT && player->y >= hitPoint->y && hitPoint -> hitPoint == 1) {
        boss->health -= 1;
        hitPoint-> hitPoint = 0;
    }
}

void attackTheBoss(SDL_Renderer *renderer, AttackFight *hitPoint, bossFight *boss, PersoFight *player) {
    if (hitPoint -> hitPoint == 1) {
        if (hitPoint->delay > 0) {
            showHitPoint(renderer, hitPoint);
            damageBoss(hitPoint, player, boss);
            hitPoint->delay -= boss->speed;
        };
    }
}





/* Fonction d'initialisation d'attaque */

AttackFight* initAttack(int x, int y, bossFight *boss) {
    AttackFight *attack = (AttackFight*)malloc(sizeof(AttackFight));
    attack->x = x;
    attack->y = y;
    attack->delay = boss->delay;
    attack->hitPoint = 1;
    return attack;
}

void resetAttack(AttackFight *attack, int delay) {
    attack->delay = delay;
}




/* Fonction d'affichage d'attaque */


int showAttack(SDL_Renderer *renderer, AttackFight *attack) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { attack->x, attack->y, TIERWIDTH, 2*QUARTERHEIGHT};
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}

int showHitPoint(SDL_Renderer *renderer, AttackFight *attack) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect = { attack->x, attack->y, TIERWIDTH, 2*QUARTERHEIGHT};
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}




/* Fonction de pattern d'attaque */

int threeTimedAttack(SDL_Renderer *renderer, PersoFight *player, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, int speed) {
    if (attack1->delay > 0 && attack2->delay > 0 && attack3->delay > 0) {
        showAttack(renderer, attack1);
        showAttack(renderer, attack2);
        showAttack(renderer, attack3);
        takeDamage(attack1, player);
        takeDamage(attack2, player);
        takeDamage(attack3, player);
        attack1->delay -= speed;
        attack2->delay -= speed;
        attack3->delay -= speed;
    }
    return 0;
}


int threeRowAttacks(SDL_Renderer *renderer, PersoFight *player, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, int speed) {
    if (attack1->delay > 0) {
        showAttack(renderer, attack1);
        takeDamage(attack1, player);
        attack1->delay -= speed;
    }
    if (attack2->delay > 0 && attack1->delay == 0) {
        showAttack(renderer, attack2);
        takeDamage(attack2, player);
        attack2->delay -= speed;
    }
    if (attack3->delay > 0 && attack2->delay == 0 && attack1->delay == 0) {
        showAttack(renderer, attack3);
        takeDamage(attack3, player);
        attack3->delay -= speed;
    }
    return 0;
}




/* Implémentation des combats de boss */

int fightBoss(SDL_Renderer *renderer, bossFight *boss, PersoFight *player, AttackFight *nullAttack, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, AttackFight *attack5, AttackFight *attack6) {
    int attackDelay1Phase1 = 1;
    int attackDelay2Phase1 = 1;
    int attackDelay3Phase1 = 1;
    int attackDelay1Phase2 = 1;
    int attackDelay2Phase2 = 1;
    int attackDelay3Phase2 = 1;
    int attackDelay1Phase3 = 1;
    int attackDelay2Phase3 = 1;
    int attackDelay3Phase3 = 1;
    
    if (boss->phase == 1) {
        if (boss->health > 6) {
            attackDelay1Phase1 = attack1->delay + attack2->delay + attack3->delay;
            printf("attackDelay1: %d\n", attackDelay1Phase1);
            if (attackDelay1Phase1 > 0) {
                threeRowAttacks(renderer, player, attack1, attack2, attack3, boss->speed);
                boss -> attack1Delay -= boss->speed;
                if (attack6 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack6, boss, player);
                }
                if (attack1 -> delay == 0 && attack2 -> delay == 0 && attack3 -> delay == 0) {
                    resetAttack(attack1, boss->delay);
                    resetAttack(attack2, boss->delay);
                    resetAttack(attack3, boss->delay);
                }
                if (attack6 -> delay == 0) {
                    resetAttack(attack6, boss->delay);
                }
            } else if (attackDelay2Phase1 > 0) {
                attackDelay2Phase1 = attack4->delay + attack5->delay + attack6->delay;
                threeRowAttacks(renderer, player, attack4, attack5, attack6, boss->speed);                
                if (attack3 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack3, boss, player);
                }
                boss -> attack2Delay -= boss->speed;
                if (attack4 -> delay == 0 && attack5 -> delay == 0 && attack6 -> delay == 0) {
                    resetAttack(attack4, boss->delay);
                    resetAttack(attack5, boss->delay);
                    resetAttack(attack6, boss->delay);
                }
                if (attack3 -> delay == 0) {
                    resetAttack(attack3, boss->delay);
                }
            } else if (attackDelay3Phase3 > 0) {
                attackDelay3Phase1 = attack1->delay + attack2->delay + attack3->delay;
                threeTimedAttack(renderer, player, attack1, attack2, attack3, boss->speed);
                if (attack5 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack5, boss, player);
                }
                boss -> attack3Delay -= boss->speed;
                if (attack1 -> delay == 0 && attack2 -> delay == 0 && attack3 -> delay == 0) {
                    resetAttack(attack1, boss->delay);
                    resetAttack(attack2, boss->delay);
                    resetAttack(attack3, boss->delay);
                }
                if (attack5 -> delay == 0) {
                    resetAttack(attack5, boss->delay);
                }
            } else {
                int attackDelay1Phase1 = attack1->delay + attack2->delay + attack3->delay;
                int attackDelay2Phase1 = attack4->delay + attack5->delay + attack6->delay;
                int attackDelay3Phase1 = attack1->delay + attack2->delay + attack3->delay;
            }
        } else {
            attack6->hitPoint = 1;
            attack3->hitPoint = 1;
            attack5->hitPoint = 1;
            boss -> phase = 2;
        }
    } else if (boss->phase == 2) {
        if (boss->health > 3) {
            if (attackDelay1Phase2 > 0) {
                threeRowAttacks(renderer, player, attack1, attack5, attack3, boss->speed);
                boss -> attack1Delay -= boss->speed;
                if (attack2 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack2, boss, player);
                }
                if (attack1 -> delay == 0 && attack5 -> delay == 0 && attack3 -> delay == 0) {
                    resetAttack(attack1, boss->delay);
                    resetAttack(attack5, boss->delay);
                    resetAttack(attack3, boss->delay);
                }
                if (attack2 -> delay == 0) {
                    resetAttack(attack2, boss->delay);
                }
            } else if (attackDelay2Phase2 > 0) {
                threeRowAttacks(renderer, player, attack4, attack2, attack6, boss->speed);
                boss -> attack2Delay -= boss->speed;
                if (attack1 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack1, boss, player);
                }
                if (attack4 -> delay == 0 && attack2 -> delay == 0 && attack6 -> delay == 0) {
                    resetAttack(attack4, boss->delay);
                    resetAttack(attack2, boss->delay);
                    resetAttack(attack6, boss->delay);
                }
                if (attack1 -> delay == 0) {
                    resetAttack(attack1, boss->delay);
                }
            } else if (attackDelay3Phase2 > 0) {
                threeTimedAttack(renderer, player, attack4, attack5, attack6, boss->speed);
                boss -> attack3Delay -= boss->speed;
                if (attack3 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack3, boss, player);
                }
                if (attack4 -> delay == 0 && attack5 -> delay == 0 && attack6 -> delay == 0) {
                    resetAttack(attack4, boss->delay);
                    resetAttack(attack5, boss->delay);
                    resetAttack(attack6, boss->delay);
                }
                if (attack3 -> delay == 0) {
                    resetAttack(attack3, boss->delay);
                }
            } else {
                attackDelay1Phase2 = boss->delay;
                attackDelay2Phase2 = boss->delay;
                attackDelay3Phase2 = boss->delay;
            }
        } else {
            attack1->hitPoint = 1;
            attack2->hitPoint = 1;
            attack3->hitPoint = 1;
            boss -> phase = 3;
        }
    } else if (boss->phase == 3) {
        if (boss->health > 0) {
            if (attackDelay1Phase3 > 0) {
                threeTimedAttack(renderer, player, attack1, nullAttack, attack4, boss->speed);
                threeTimedAttack(renderer, player, attack3, nullAttack, attack6, boss->speed);
                boss -> attack1Delay -= boss->speed;
                if (attack5 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack5, boss, player);
                }
                if (attack1 -> delay == 0 && attack4 -> delay == 0 && attack6 -> delay == 0 && attack3 -> delay == 0 && attack6 -> delay == 0) {
                    resetAttack(attack1, boss->delay);
                    resetAttack(attack4, boss->delay);
                    resetAttack(attack6, boss->delay);
                    resetAttack(attack3, boss->delay);
                }
                if (attack5 -> delay == 0) {
                    resetAttack(attack5, boss->delay);
                }
            } else if (attackDelay2Phase3 > 0) {
                threeTimedAttack(renderer, player, attack5, nullAttack, attack3, boss->speed);
                threeTimedAttack(renderer, player, attack2, nullAttack, attack6, boss->speed);
                boss -> attack2Delay -= boss->speed;
                if (attack4 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack4, boss, player);
                }
                if (attack5 -> delay == 0 && attack3 -> delay == 0 && attack6 -> delay == 0 && attack2 -> delay == 0 && attack4 -> delay == 0) {
                    resetAttack(attack5, boss->delay);
                    resetAttack(attack3, boss->delay);
                    resetAttack(attack6, boss->delay);
                    resetAttack(attack2, boss->delay);
                }
                if (attack4 -> delay == 0) {
                    resetAttack(attack4, boss->delay);
                }
            } else if (attackDelay3Phase3 > 0) {
                threeTimedAttack(renderer, player, attack5, nullAttack, attack4, boss->speed);
                threeTimedAttack(renderer, player, attack2, nullAttack, attack1, boss->speed);
                boss -> attack3Delay -= boss->speed;
                if (attack6 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack6, boss, player);
                }
                if (attack5 -> delay == 0 && attack4 -> delay == 0 && attack1 -> delay == 0 && attack2 -> delay == 0) {
                    resetAttack(attack5, boss->delay);
                    resetAttack(attack4, boss->delay);
                    resetAttack(attack1, boss->delay);
                    resetAttack(attack2, boss->delay);
                }
                if (attack6 -> delay == 0) {
                    resetAttack(attack6, boss->delay);
                }
            } else {
                attackDelay1Phase3 = boss->delay;
                attackDelay2Phase3 = boss->delay;
                attackDelay3Phase3 = boss->delay;
            }
        } else {
            attack4->hitPoint = 1;
            attack5->hitPoint = 1;
            attack6->hitPoint = 1;
            boss -> phase = 0;
        }
    } else if (boss->phase == 0) {
        if (boss->health == 0) {
            printf("You won\n");
            exit(0);
        }
    }
    return 0;
}