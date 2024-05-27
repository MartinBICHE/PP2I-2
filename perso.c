#include "perso.h"

void display_projectile(SDL_Renderer *renderer, Projectile_Perso *projectile_perso) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur rouge pour le projectile
    SDL_Rect rect = {.x = projectile_perso->x, .y = projectile_perso->y, .w = PROJECTILE_WIDTH, .h = PROJECTILE_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
}


Perso *create_perso(Map *map) {
    Perso *res = malloc(sizeof(Perso));
    res->x = map->start_x;
    /* res->x = 3 * map->pix_rect; */
    res->y = map->start_y;
    res->vx = 0;
    res->vy = 0;
    res->facing = 1;
    res->hitbox = (SDL_Rect){.x = (res->x - PERSO_WIDTH/2.0f)*map->pix_rect, .y = (res->y - PERSO_HEIGHT/2.0f)*map->pix_rect, .w = PERSO_WIDTH*map->pix_rect, .h = PERSO_HEIGHT*map->pix_rect};
    res->spriteOffset = 0;
    res->health = 9;
    res->jumps = 2;
    res->jump_delay = 0;
    res->dashes = 2;
    res->dash_duration = 0;
    res->dash_speed = 21.0f;
    res->dash_delay = 0;
    return res;
}

void loadPersoTexture(SDL_Renderer *renderer, SDL_Texture **persoTexture, char *path) {
	SDL_Surface *persoSurface = IMG_Load(path);
	if (!persoSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in perso surface init %s", IMG_GetError());
		exit(-1);
	}
	*persoTexture = SDL_CreateTextureFromSurface(renderer, persoSurface);
	if (!*persoTexture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in perso texture init: %s", SDL_GetError());
		exit(-1);
	}
    // SDL_SetTextureScaleMode(*persoTexture, SDL_ScaleModeLinear);
	SDL_FreeSurface(persoSurface);
}

void loadAttackTexture(SDL_Renderer *renderer, SDL_Texture **attackTexture, char *path) {
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


int display_perso_hitbox(SDL_Renderer *renderer, Perso *perso, Map *map) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    SDL_Rect rect1 = {.x = perso->hitbox.x - map->x_cam, .y = perso->hitbox.y, .w = perso->hitbox.w, .h = perso->hitbox.h};
    if (SDL_RenderDrawRect(renderer, &rect1)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in draw rect : %s", SDL_GetError());
		exit(-1);
    }
    return 0;
}


int hitbox_bottom(Perso *perso, Map *map) {
    SDL_Rect hbb = {.x = perso->hitbox.x + 2, .y = perso->hitbox.y + perso->hitbox.h + 3, .w = perso->hitbox.w - 4, .h = 1};
    SDL_Rect res;
    int i = perso->y + 1;
    int j = perso->x;
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbb, &rect, &res)) return 1;
    }
    return 0;
}


int display_perso(SDL_Renderer *renderer, Perso *perso, Map *map, SDL_Texture *persoTexture, int showHitbox, Mix_Chunk **sounds) {
    int c = 96; // côté du carré de destination du sprite du perso
    int centrage = 6;
    if (currentGravity < 0) {
        centrage = -centrage;
    }
    SDL_Rect dst_rect = {.x = perso->x*map->pix_rect - map->x_cam - c/2, .y = perso->y*map->pix_rect - c/2 - centrage, .w = c, .h = c};
    double angle = (currentGravity < 0) ? 180.0 : 0.0;
    SDL_RendererFlip flip = (perso->facing == 1) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    if (perso->dash_duration == 10) {
        int channel = Mix_PlayChannel(-1, sounds[2], 0);
        if(channel == -1) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error playing dashing sound: %s", Mix_GetError());
            exit(-1);
        }
    }
    if (perso->dash_duration > 0) { // perso en train de dasher
    
        int offset = 45; // décalage en x pour les "rémanences"
        if (currentGravity < 0) {
            offset = -offset;
        }
        SDL_Rect src_rect;
        if (currentGravity > 0) {
            if (hitbox_bottom(perso, map)) {
                perso->spriteOffset = (perso->spriteOffset + 1) % 24; // 6 frames par sprite, 4 sprites
                src_rect = (SDL_Rect){.x = (perso->spriteOffset/6)*64, .y = 3*64, .w = 64, .h = 64};
            } else {
                src_rect = (SDL_Rect){.x = 6*64, .y = 2*64, .w = 64, .h = 64};
            }
        } else {
            if (hitbox_top(perso, map)) {
                perso->spriteOffset = (perso->spriteOffset + 1) % 24; // 6 frames par sprite, 4 sprites
                src_rect = (SDL_Rect){.x = (perso->spriteOffset/6)*64, .y = 3*64, .w = 64, .h = 64};
            } else {
                src_rect = (SDL_Rect){.x = 6*64, .y = 2*64, .w = 64, .h = 64};
            }
        }

        if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect, angle, NULL, flip)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
            exit(-1);
        }

        if (perso->dash_duration <= 4) {
            SDL_Rect dst_rect1 = {.x = perso->x*map->pix_rect - map->x_cam - c/2 - 3*offset*perso->facing, .y = perso->y*map->pix_rect - c/2 - centrage, .w = c, .h = c}; // rémanence très loin derrière
            SDL_SetTextureAlphaMod(persoTexture, 63);
            if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect1, angle, NULL, flip)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                exit(-1);
            }
        }

        if (perso->dash_duration <= 7) {
            SDL_Rect dst_rect2 = {.x = perso->x*map->pix_rect - map->x_cam - c/2 - 2*offset*perso->facing, .y = perso->y*map->pix_rect - c/2 - centrage, .w = c, .h = c}; // rémanence loin derrière
            SDL_SetTextureAlphaMod(persoTexture, 95);
            if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect2, angle, NULL, flip)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
                exit(-1);
            }
        }

        SDL_Rect dst_rect3 = {.x = perso->x*map->pix_rect - map->x_cam - c/2 - offset*perso->facing, .y = perso->y*map->pix_rect - c/2 - centrage, .w = c, .h = c}; // rémanence proche derrière
        SDL_SetTextureAlphaMod(persoTexture, 159);
        if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect3, angle, NULL, flip)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
            exit(-1);
        }

        SDL_SetTextureAlphaMod(persoTexture, 255);
    } else if (perso->vy != 0) { // perso en train de sauter
        perso->spriteOffset = (perso->spriteOffset + 1) % 42; // 6 frames par sprite, 7 sprites
        SDL_Rect src_rect = {.x = (perso->spriteOffset/6)*64, .y = 2*64, .w = 64, .h = 64};
        if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect, angle, NULL, flip)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
            exit(-1);
        }

    } else if (perso->vx != 0) { // perso en train de marcher
        if (perso->spriteOffset == 24) {
            int channel = Mix_PlayChannel(-1, sounds[0], 0);
            if(channel == -1) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error playing walking sound: %s", Mix_GetError());
                exit(-1);
            }
        } else if (perso->spriteOffset == 60) {
            int channel = Mix_PlayChannel(-1, sounds[1], 0);
            if(channel == -1) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error playing walking sound: %s", Mix_GetError());
                exit(-1);
            }
        }
        perso->spriteOffset = (perso->spriteOffset + 1) % 72; // 6 frames par sprite, 12 
        SDL_Rect src_rect;
        // if (currentGravity > 0) {
            src_rect = (SDL_Rect){.x = (perso->spriteOffset/6)*64, .y = 64, .w = 64, .h = 64};
        // } else {
        //     src_rect = (SDL_Rect){.x =64, .y = (perso->spriteOffset/6)*64, .w = 64, .h = 64};
        // }
        if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect, angle, NULL, flip)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
            exit(-1);
        }
    } else { // perso immobile
        perso->spriteOffset = (perso->spriteOffset + 1) % 72; // 6 frames par sprite, 12 sprites
        SDL_Rect src_rect = {.x = (perso->spriteOffset/6)*64, .y = 0, .w = 64, .h = 64};
        if (SDL_RenderCopyEx(renderer, persoTexture, &src_rect, &dst_rect, angle, NULL, flip)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in render copy: %s", SDL_GetError());
            exit(-1);
        }
    }

    if (showHitbox) {
        if (display_perso_hitbox(renderer, perso, map)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in display perso hitbox: %s", SDL_GetError());
            exit(-1);
        }
    }

    return 0;
}




int hitbox_top(Perso *perso, Map *map) {
    SDL_Rect hbt = {.x = perso->hitbox.x + 1, .y = perso->hitbox.y - 3, .w = perso->hitbox.w - 2, .h = 1};
    SDL_Rect res;
    int i = perso->y - 1;
    int j = perso->x;
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbt, &rect, &res)) return 1;
    }
    return 0;
}

int hitbox_left(Perso *perso, Map *map) {
    SDL_Rect hbl = {.x = perso->hitbox.x - 3, .y = perso->hitbox.y + 7, .w = 3, .h = perso->hitbox.h - 12};
    SDL_Rect res;
    int i = perso->y;
    int j = perso->x - 1;
    if (map->matrix[i][j+1] != '-') {
        SDL_Rect rect = {.x = (j+1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i-1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i-1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    if (map->matrix[i+1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i+1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbl, &rect, &res)) return 1;
    }
    return 0;
}


int hitbox_right(Perso *perso, Map *map) {
    SDL_Rect hbr = {.x = perso->hitbox.x + perso->hitbox.w + 1, .y = perso->hitbox.y + 7, .w = 3, .h = perso->hitbox.h - 12};
    SDL_Rect res;
    int i = perso->y;
    int j = perso->x + 1;
    if (map->matrix[i][j-1] != '-') {
        SDL_Rect rect = {.x = (j-1)*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = i*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i-1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i-1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    if (map->matrix[i+1][j] != '-') {
        SDL_Rect rect = {.x = j*map->pix_rect, .y = (i+1)*map->pix_rect, .w = map->pix_rect, .h = map->pix_rect};
        if (SDL_IntersectRect(&hbr, &rect, &res)) return 1;
    }
    return 0;
}

int hitbox_enemy(Perso *perso, Map *map, EnemyStateData *enemyStateData) {
    SDL_Rect enemyHitbox = enemyStateData->dst_rect;
    int margin = 10; // Marge pour que le personnage ne soit pas collé à la hitbox de l'ennemi
    enemyHitbox.x -= margin;
    enemyHitbox.y -= margin;
    enemyHitbox.w += 2 * margin;
    enemyHitbox.h += 2 * margin;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&perso->hitbox, &enemyHitbox, &intersection)) { // Détecte si le personnage rencontre l'ennemi
        return 1;
    }
    return 0;
}


float max(float a, float b) {
    if (a<b)return b;
    else return a;
}


float min(float a, float b) {
    if (a>b)return b;
    else return a;
}

void jump(Perso *perso, Map *map) {
    if (perso->jumps > 0 && perso->jump_delay == 0) {
        if (currentGravity < 0) {
            perso->vy = -JUMPSPEED_INVERTED;
        } else {
            perso->vy = -JUMPSPEED;
        }
        perso->jumps--;
        perso->jump_delay = 22;
    }
}


void persoAttack(Perso *perso, EnemyStateData *enemyStateData) {
    SDL_Rect attackHitbox = perso->hitbox;
    int attackRange = 20; // Portée de l'attaque
    attackHitbox.x += perso->facing * attackRange;
    SDL_Rect intersection;
    if (SDL_IntersectRect(&attackHitbox, &enemyStateData->dst_rect, &intersection)) {
        static int i = 0;
        printf("Attaque %d\n", i++);
        printf("Santé ennemi : %d\n", enemyStateData->health);
        enemyStateData->health -= 1; // Infliger 1 point de dégât
        printf("Santé ennemi : %d\n", enemyStateData->health);
        if (enemyStateData->health <= 0) {
            // Faire disparaître l'ennemi, par exemple en le déplaçant hors de l'écran
            enemyStateData->dst_rect.x = -1000;
            enemyStateData->dst_rect.y = -1000;
        }
    }
}

void distanceAttack(Perso *perso, EnemyStateData *enemyStateData, Map *map, Projectile_Perso *projectile_perso) {
    int attack_range = 300; // Portée de l'attaque à distance
    float dx = (perso->x * map->pix_rect) - enemyStateData->dst_rect.x;
    float dy = (perso->y * map->pix_rect) - enemyStateData->dst_rect.y;
    float distance = sqrt(dx * dx + dy * dy);
    if (distance <= attack_range) {
        // L'ennemi est dans la portée d'attaque à distance
        // Définir la position initiale du projectile
        projectile_perso->x = perso->x;
        projectile_perso->y = perso->y;
        // Définir la direction du projectile (vers l'ennemi)
        projectile_perso->direction_x = dx / distance;
        projectile_perso->direction_y = dy / distance;
        // Définir la vitesse du projectile
        projectile_perso->speed = 5.0f; // Modifiez la vitesse selon vos besoins
        display_projectile(renderer, projectile_perso);
        static int j = 0;
        printf("Attaque à distance %d\n", j++);
        enemyStateData->health -= 1; // Infliger 1 point de dégât
        if (enemyStateData->health <= 0) {
            // Faire disparaître l'ennemi, par exemple en le déplaçant hors de l'écran
            enemyStateData->dst_rect.x = -1000;
            enemyStateData->dst_rect.y = -1000;
        }
    }
}

void changeGravity(void) {
	if (currentGravity == ACC) {
		currentGravity = ACC_INVERTED;
		jumpSpeed = JUMPSPEED_INVERTED;
	} else {
		currentGravity = ACC;
		jumpSpeed = JUMPSPEED;
	}
}

void updateHitbox(Perso *perso, Map *map) {
    perso->hitbox = (SDL_Rect){.x = (perso->x - PERSO_WIDTH/2.0f)*map->pix_rect, .y = (perso->y - PERSO_HEIGHT/2.0f)*map->pix_rect, .w = PERSO_WIDTH*map->pix_rect, .h = PERSO_HEIGHT*map->pix_rect};
}

void updatePersoDashing(Perso *perso, Map *map) {
    perso->vy = 0;
    int j = floor(perso->x);
    if (currentGravity > 0) {
        perso->vx = perso->dash_speed*perso->facing;
    } else {
        perso->vx = -perso->dash_speed*perso->facing;
    }
    perso->x += perso->vx*DT;
    updateHitbox(perso, map);
    if (currentGravity > 0) {
        if (perso->facing == -1 && hitbox_left(perso, map)) {
            perso->x = j + 0.33f;
            perso->dash_duration = 0;
        }
        if (perso->facing == 1 && hitbox_right(perso, map)) {
            perso->x = j+1 - 0.32f;
            perso->dash_duration = 0;
        }
    } else {
        if (perso->facing == 1 && hitbox_left(perso, map)) {
            perso->x = j + 0.33f;
            perso->dash_duration = 0;
        }
        if (perso->facing == -1 && hitbox_right(perso, map)) {
            perso->x = j+1 - 0.32f;
            perso->dash_duration = 0;
        }
    }
    updateHitbox(perso, map);
}


void updatePerso(Perso *perso, Map *map, EnemyStateData *enemyStateData, const Uint8 *state, Mix_Chunk **sounds) {
    perso->jump_delay = max(perso->jump_delay - 1, 0);
    perso->dash_duration = max(perso->dash_duration - 1, 0);
    perso->dash_delay = max(perso->dash_delay - 1, 0);
    static int k_pressed_this_frame = 0; // Variable locale pour suivre l'état de la touche K dans cette frame
    static int l_pressed_this_frame = 0; // Variable locale pour suivre l'état de la touche L dans cette frame
    if (perso->dash_duration > 0) updatePersoDashing(perso, map);
    else {
        if (perso->recoil_timer > 0) {
            perso->recoil_timer--;
        } else {
            perso->vx = 0;
            if (state[SDL_SCANCODE_A]) { // Q en AZERTY
                if (currentGravity > 0) {
                    perso->facing = -1;
                } else {
                    perso->facing = 1;
                }
                perso->vx -= MOOVSPEED;
            }
            if (state[SDL_SCANCODE_D]) {
                if (currentGravity > 0) {
                    perso->facing = 1;
                } else {
                    perso->facing = -1;
                }
                perso->vx += MOOVSPEED;
            }
        
            if (state[SDL_SCANCODE_SPACE]) jump(perso, map);
        }
        if (state[SDL_SCANCODE_J] && perso->dash_delay == 0 && perso->dashes > 0) {
            perso->dashes--;
            perso->dash_duration = 11;
            perso->dash_delay = 30;
        }
        if (state[SDL_SCANCODE_K] && !k_pressed_this_frame) {
            persoAttack(perso, enemyStateData);
            // Marquer que la touche K a été pressée dans cette frame
            k_pressed_this_frame = 1;
        }
        // Réinitialiser la variable locale si la touche K est relâchée
        if (!state[SDL_SCANCODE_K]) {
            k_pressed_this_frame = 0;
        }
        if (state[SDL_SCANCODE_L] && !l_pressed_this_frame) {
            Projectile_Perso *projectile_perso = malloc(sizeof(Projectile_Perso));
            distanceAttack(perso, enemyStateData, map, projectile_perso);
            // Marquer que la touche L a été pressée dans cette frame
            l_pressed_this_frame = 1;
        }
        // Réinitialiser la variable locale si la touche L est relâchée
        if (!state[SDL_SCANCODE_L]) {
            l_pressed_this_frame = 0;
        }

        int i = floor(perso->y);
        int j = floor(perso->x);
        perso->vy += currentGravity*DT;
        int v = 17; // vitesse à partir de laquelle le son "crack" est joué quand le perso s'écrase au sol
        if (currentGravity > 0) {
            if (hitbox_bottom(perso, map)) {
                if (perso->vy > v) {
                    int channel = Mix_PlayChannel(-1, sounds[3], 0);
                    if(channel == -1) {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error playing crack sound: %s", Mix_GetError());
                        exit(-1);
                    }
                }
                perso->vy = min(perso->vy, 0.0f);
                perso->y = i + 1 - PERSO_HEIGHT/2.0f;
            }
            if (hitbox_top(perso, map)) {
                perso->vy = max(perso->vy, 0.0f);
                perso->y = i + PERSO_HEIGHT/2.0f;
            }
        } else {
            if (hitbox_bottom(perso, map)) {
                perso->vy = min(perso->vy, 0.0f);
                perso->y = i + 1 - PERSO_HEIGHT/2.0f;
            }
            if (hitbox_top(perso, map)) {
                if (perso->vy > v) {
                    int channel = Mix_PlayChannel(-1, sounds[3], 0);
                    if(channel == -1) {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error playing crack sound: %s", Mix_GetError());
                        exit(-1);
                    }
                }
                perso->vy = max(perso->vy, 0.0f);
                perso->y = i + PERSO_HEIGHT/2.0f;
                perso->jumps = 1;
            }
        }
        if (hitbox_left(perso, map)) {
            perso->vx = max(perso->vx, 0.0f);
            perso->x = j + PERSO_WIDTH/2.0f;
        }
        if (hitbox_right(perso, map)) {
            perso->vx = min(perso->vx, 0.0f);
            perso->x = j+1 - PERSO_WIDTH/2.0f;
        }
        perso->y += perso->vy*DT;
        perso->x += perso->vx*DT;
        updateHitbox(perso, map);
        if (hitbox_bottom(perso, map)) {
            perso->jumps = 2;
            perso->dashes = 2;
        }
    }
}

