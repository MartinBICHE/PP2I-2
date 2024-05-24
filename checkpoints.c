#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H
#include "const.h"
#include <stdio.h>
#include "checkpoints.h"
#define CAPACITY 10



void initCheckpointList(CheckpointList *list){
    list->xPositions = malloc(sizeof(int) * CAPACITY);
    if (list->xPositions == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        exit(-1);
    }
    list->size = 0;
    list->capacity = CAPACITY;
}

void addCheckpoint(CheckpointList *list, int position) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->xPositions = realloc(list->xPositions, list->capacity * sizeof(int));
        if (list->xPositions == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    list->xPositions[list->size++] = position;
}


void savePosition(const char *filename, Perso *perso){
    FILE *file = fopen(filename, "wb");
    if (file != NULL){
        fwrite(perso, sizeof(Perso), 1, file);
        fclose(file);
    }
}

void loadPosition(const char *filename, Perso *perso){
    FILE *file = fopen(filename, "rb");
    if (file != NULL){
        if (1 != fread(perso, sizeof(Perso), 1, file))
        fclose(file);
    }
}

void checkCheckpoint(CheckpointList *list, Perso *perso, const char *filename) {
    double position_tolerance = 0.5;
    for (int i = 0; i < list->size; i++) {
        if (fabs(perso->x - list->xPositions[i]) <= position_tolerance) {
            savePosition(filename, perso);
            puts("yepee");
            printf("%f\n", perso->x);
        }
    }
}


void free_checkpoints(CheckpointList *list){
    free(list->xPositions);
    free(list);
}

#endif
