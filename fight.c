#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "const.h"
#include "fight.h"
#include "menu.h"

/* Fonction de gestion du personnage */

int fightMovement(SDL_Renderer *renderer, PersoFight *player) {
    static int offset = 0;
    static int line = 4;

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

    int spriteHeight = spriteFullHeight / 5; 
    int spriteWidth = spriteFullWidth / 12;

    const int INPUT_DELAY_MS = 5; 
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
        offset = (offset+1) % 9;
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
        player->iframe = 100;
    }
    if (player->health == 0) {
        printf("You died\n");
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

void attackTheBoss(SDL_Renderer *renderer, AttackFight *hitPoint, bossFight *boss, PersoFight *player, Animation *anim) {
    if (hitPoint -> hitPoint == 1) {
        showHitPoint(renderer, hitPoint, anim);
        damageBoss(hitPoint, player, boss);
    }
}





/* Fonction d'initialisation d'attaque */

AttackFight* initAttack(int x, int y, bossFight *boss) {
    AttackFight *attack = (AttackFight*)malloc(sizeof(AttackFight));
    attack->x = x;
    attack->y = y;
    attack->warning = 0.2*boss->delay;
    attack->delay =  0.8*boss->delay;
    attack->hitPoint = 1;
    return attack;
}

void resetAttack(AttackFight *attack, int delay) {
    attack->delay = 0.8*delay;
    attack->warning = 0.2*delay;
}


/* Fonction d'animation */

void loadDeathTexture(SDL_Renderer *renderer, SDL_Texture **attackTexture, char *path) {
	SDL_Surface *attackSurface = IMG_Load(path);
	if (!attackSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in perso surface init %s", IMG_GetError());
		exit(-1);
	}
	*attackTexture = SDL_CreateTextureFromSurface(renderer, attackSurface);
	if (!*attackTexture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in perso texture init: %s", SDL_GetError());
		exit(-1);
	}
    // SDL_SetTextureScaleMode(*persoTexture, SDL_ScaleModeLinear);
	SDL_FreeSurface(attackSurface);
}

void initAnimation(Animation* anim, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Uint32 frameDuration) {
    anim->texture = texture;
    anim->frameWidth = frameWidth/numFrames;
    anim->frameHeight = frameHeight;
    anim->numFrames = numFrames;
    anim->currentFrame = 0;
    anim->frameDuration = frameDuration;
    anim->lastFrameTime = SDL_GetTicks();
}

void updateAttackAnimation(Animation* anim) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - anim->lastFrameTime > anim->frameDuration) {
        if (anim -> currentFrame != anim -> numFrames) {
            anim->currentFrame = (anim->currentFrame + 1);
            anim->lastFrameTime = currentTime;
        }
    }
}

void resetAnimation(Animation *anim) {
        anim -> currentFrame = 0;
}

void updateLoopAnimation(Animation* anim) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - anim->lastFrameTime > anim->frameDuration) {
        anim->currentFrame = (anim->currentFrame + 1) % anim->numFrames;
        anim->lastFrameTime = currentTime;
    }
}

void renderAnimation(SDL_Renderer* renderer, Animation* anim, int x, int y) {
    SDL_Rect srcRect = {
        anim->currentFrame * anim->frameWidth,
        0,
        anim->frameWidth,
        anim->frameHeight
    };
    SDL_Rect dstRect = { x, y, TIERWIDTH, QUARTERHEIGHT*2 };
    SDL_RenderCopy(renderer, anim->texture, &srcRect, &dstRect);
}

void renderBackgroundAnimation(SDL_Renderer* renderer, Animation* anim) {
    SDL_Rect srcRect = {
        anim->currentFrame * anim->frameWidth,
        0,
        anim->frameWidth,
        anim->frameHeight
    };
    SDL_Rect dstRect = { 0, 0, WINWIDTH, WINHEIGHT};
    SDL_RenderCopy(renderer, anim->texture, &srcRect, &dstRect);
}


void animateBackground(SDL_Renderer *renderer, Animation *anim) {
    renderBackgroundAnimation(renderer, anim);
    updateLoopAnimation(anim);
}


/* Fonction d'affichage d'attaque */

int showAttack(SDL_Renderer *renderer, AttackFight *attack, Animation *anim) {
    renderAnimation(renderer, anim, attack->x, attack->y);
    updateAttackAnimation(anim);
    return 0;
}

int showHitPoint(SDL_Renderer *renderer, AttackFight *attack, Animation *anim) {
    renderAnimation(renderer, anim, attack->x, attack->y);
    updateLoopAnimation(anim);
    return 0;
}

int showAttackWarning(SDL_Renderer *renderer, AttackFight *attack) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
    SDL_Rect rect = { attack->x, attack->y, TIERWIDTH, 2*QUARTERHEIGHT};
    SDL_RenderFillRect(renderer, &rect);
    return 0;
}




/* Fonction de pattern d'attaque */

int threeTimedAttack(SDL_Renderer *renderer, PersoFight *player, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, bossFight *boss, Animation *anim) {
    if (attack1->delay > 0 && attack2->delay > 0 && attack3->delay > 0) {
        if (attack1->warning > 0 && attack2->warning > 0 && attack3->warning > 0) {
            showAttackWarning(renderer, attack1);
            showAttackWarning(renderer, attack2);
            showAttackWarning(renderer, attack3);
            attack1->warning -= boss -> speed;
            attack2->warning -= boss -> speed;
            attack3->warning -= boss -> speed;
        } else {
            showAttack(renderer, attack1, anim);
            showAttack(renderer, attack2, anim);
            showAttack(renderer, attack3, anim);
            takeDamage(attack1, player);
            takeDamage(attack2, player);
            takeDamage(attack3, player);
            attack1->delay -= boss -> speed;
            attack2->delay -= boss -> speed;
            attack3->delay -= boss -> speed;
        }
    } else if (attack1->delay == 0 && attack2->delay == 0 && attack3->delay == 0) {
        resetAttack(attack1, boss -> delay);
        resetAttack(attack2, boss -> delay);
        resetAttack(attack3, boss -> delay);
        resetAnimation(anim);
    }
    return 0;
}

int fourTimedAttack(SDL_Renderer *renderer, PersoFight *player, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, bossFight *boss, Animation *anim) {
    if (attack1->delay > 0 && attack2->delay > 0 && attack3->delay > 0 && attack4->delay > 0) {
        if (attack1->warning > 0 && attack2->warning > 0 && attack3->warning > 0 && attack4->warning > 0) {
            showAttackWarning(renderer, attack1);
            showAttackWarning(renderer, attack2);
            showAttackWarning(renderer, attack3);
            showAttackWarning(renderer, attack4);
            attack1->warning -= boss -> speed;
            attack2->warning -= boss -> speed;
            attack3->warning -= boss -> speed;
            attack4->warning -= boss -> speed;
        } else {
            showAttack(renderer, attack1, anim);
            showAttack(renderer, attack2, anim);
            showAttack(renderer, attack3, anim);
            showAttack(renderer, attack4, anim);
            takeDamage(attack1, player);
            takeDamage(attack2, player);
            takeDamage(attack3, player);
            takeDamage(attack4, player);
            attack1->delay -= boss -> speed;
            attack2->delay -= boss -> speed;
            attack3->delay -= boss -> speed;
            attack4->delay -= boss -> speed;
        }
    } else {
        resetAttack(attack1, boss -> delay);
        resetAttack(attack2, boss -> delay);
        resetAttack(attack3, boss -> delay);
        resetAttack(attack4, boss -> delay);
        resetAnimation(anim);
    }
    return 0;
}


int threeRowAttacks(SDL_Renderer *renderer, PersoFight *player, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, bossFight *boss, Animation *anim) {
    if (attack1->delay > 0) {
        if (attack1->warning > 0) {
            resetAnimation(anim);
            showAttackWarning(renderer, attack1);
            attack1->warning -= boss -> speed;
        } else {
            showAttack(renderer, attack1, anim);
            takeDamage(attack1, player);
            attack1->delay -= boss -> speed;
        }
    }
    if (attack2->delay > 0 && attack1->delay == 0) {
        if (attack2->warning > 0) {
            resetAnimation(anim);
            showAttackWarning(renderer, attack2);
            attack2->warning -= boss -> speed;
        } else {
            showAttack(renderer, attack2, anim);
            takeDamage(attack2, player);
            attack2->delay -= boss -> speed;
        }
    }
    if (attack3->delay > 0 && attack2->delay == 0 && attack1->delay == 0) {
        if (attack3->warning > 0) {
            resetAnimation(anim);
            showAttackWarning(renderer, attack3);
            attack3->warning -= boss -> speed;
        } else {
            showAttack(renderer, attack3, anim);
            takeDamage(attack3, player);
            attack3->delay -=boss ->  speed;
        }
    }
    if (attack1->delay == 0 && attack2->delay == 0 && attack3->delay == 0) {
        resetAttack(attack1, boss -> delay);
        resetAttack(attack2, boss -> delay);
        resetAttack(attack3, boss -> delay);
        resetAnimation(anim);
    }
    return 0;
}




/* Implémentation des combats de boss */

int fightBoss(SDL_Renderer *renderer, bossFight *boss, PersoFight *player, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, AttackFight *attack5, AttackFight *attack6, Animation *scratchAnim, Animation *hitpointAnim) {
    int attackDelay1Phase1 = 3*boss->attack1Delay;
    int attackDelay2Phase1 = 3*boss->attack2Delay;
    int attackDelay3Phase1 = boss->attack3Delay / 6;
    int attackDelay1Phase2 = 3*boss->attack1Delay;
    int attackDelay2Phase2 = 3*boss->attack2Delay;
    int attackDelay3Phase2 = boss->attack3Delay / 6;
    int attackDelay1Phase3 = boss->attack1Delay / 6;
    int attackDelay2Phase3 = boss->attack2Delay / 6;
    int attackDelay3Phase3 = boss->attack3Delay / 6;
    
    if (boss->phase == 1) {
        if (boss->health > 6) {
            if (attackDelay1Phase1 > 0) {
                threeRowAttacks(renderer, player, attack1, attack2, attack3, boss, scratchAnim);
                boss -> attack1Delay -= boss->speed;
                if (attack6 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack6, boss, player, hitpointAnim);
                }
            } else if (attackDelay2Phase1 > 0) {
                threeRowAttacks(renderer, player, attack4, attack5, attack6, boss, scratchAnim); 
                boss -> attack2Delay -= boss->speed;               
                if (attack3 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack3, boss, player, hitpointAnim);
                }
            } else if (attackDelay3Phase1 > 0) {
                threeTimedAttack(renderer, player, attack1, attack2, attack3, boss, scratchAnim);
                boss -> attack3Delay -= boss->speed;
                if (attack5 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack5, boss, player, hitpointAnim);
                }
            } else if (attackDelay1Phase1 == 0 && attackDelay2Phase1 == 0 && attackDelay3Phase1 == 0) {
                boss -> attack1Delay = 3*boss->delay;
                boss -> attack2Delay = 3*boss->delay;
                boss -> attack3Delay = 3*boss->delay;
                attackDelay1Phase1 = 3*boss-> attack1Delay;
                attackDelay2Phase1 = 3*boss-> attack2Delay;
                attackDelay3Phase1 = boss-> attack3Delay / 6;
                resetAttack(attack1, boss->delay);
                resetAttack(attack2, boss->delay);
                resetAttack(attack3, boss->delay);
                resetAttack(attack4, boss->delay);
                resetAttack(attack5, boss->delay);
                resetAttack(attack6, boss->delay);
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
                threeRowAttacks(renderer, player, attack1, attack5, attack3, boss, scratchAnim);
                boss -> attack1Delay -= boss->speed;
                if (attack2 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack2, boss, player, hitpointAnim);
                }
            } else if (attackDelay2Phase2 > 0) {
                threeRowAttacks(renderer, player, attack4, attack2, attack6, boss, scratchAnim);
                boss -> attack2Delay -= boss->speed;
                if (attack1 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack1, boss, player, hitpointAnim);
                }
            } else if (attackDelay3Phase2 > 0) {
                threeTimedAttack(renderer, player, attack4, attack5, attack6, boss, scratchAnim);
                boss -> attack3Delay -= boss->speed;
                if (attack3 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack3, boss, player, hitpointAnim);
                }
            } else {
                boss -> attack1Delay = 3*boss->delay;
                boss -> attack2Delay = 3*boss->delay;
                boss -> attack3Delay = 3*boss->delay;
                attackDelay1Phase2 = 3*boss-> attack1Delay;
                attackDelay2Phase2 = 3*boss-> attack2Delay;
                attackDelay3Phase2 = boss-> attack3Delay / 3;
                resetAttack(attack1, boss->delay);
                resetAttack(attack2, boss->delay);
                resetAttack(attack3, boss->delay);
                resetAttack(attack4, boss->delay);
                resetAttack(attack5, boss->delay);
                resetAttack(attack6, boss->delay);
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
                fourTimedAttack(renderer, player, attack1, attack3, attack4, attack6, boss, scratchAnim);
                boss -> attack1Delay -= boss->speed;
                if (attack5 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack5, boss, player, hitpointAnim);
                }
            } else if (attackDelay2Phase3 > 0) {
                fourTimedAttack(renderer, player, attack5, attack2, attack6, attack3, boss, scratchAnim);
                boss -> attack2Delay -= boss->speed;
                if (attack4 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack4, boss, player, hitpointAnim);
                }
            } else if (attackDelay3Phase3 > 0) {
                fourTimedAttack(renderer, player, attack5, attack1, attack4, attack2, boss, scratchAnim);
                boss -> attack3Delay -= boss->speed;
                if (attack6 -> hitPoint == 1) {
                    attackTheBoss(renderer, attack6, boss, player, hitpointAnim);
                }
            } else {
                boss -> attack1Delay = 3*boss->delay;
                boss -> attack2Delay = 3*boss->delay;
                boss -> attack3Delay = 3*boss->delay;
                attackDelay1Phase3 = boss-> attack1Delay ;
                attackDelay2Phase3 = boss-> attack2Delay ;
                attackDelay3Phase3 = boss-> attack3Delay ;
                resetAttack(attack1, boss->delay);
                resetAttack(attack2, boss->delay);
                resetAttack(attack3, boss->delay);
                resetAttack(attack4, boss->delay);
                resetAttack(attack5, boss->delay);
                resetAttack(attack6, boss->delay);
            }
        } else {
            attack4->hitPoint = 1;
            attack5->hitPoint = 1;
            attack6->hitPoint = 1;
            boss -> phase = 0;
        }
    }
    return 0;
}


void resetGameplay2(bossFight *boss, AttackFight *attack1, AttackFight *attack2, AttackFight *attack3, AttackFight *attack4, AttackFight *attack5, AttackFight *attack6) {
    resetAttack(attack1, 3*boss -> delay);
    resetAttack(attack2, 3*boss -> delay);
    resetAttack(attack3, 3*boss -> delay);
    resetAttack(attack4, 3*boss -> delay);
    resetAttack(attack5, 3*boss -> delay);
    resetAttack(attack6, 3*boss -> delay);
    attack1->hitPoint = 1;
    attack2->hitPoint = 1;
    attack3->hitPoint = 1;
    attack4->hitPoint = 1;
    attack5->hitPoint = 1;
    attack6->hitPoint = 1;
}
