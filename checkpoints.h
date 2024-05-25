#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H
#include "const.h"

struct _CheckpointList{
    int *xPositions;
    int size;
    int capacity;
};

typedef struct _CheckpointList CheckpointList;
void savePosition(const char *filename, Perso *perso);
void loadPosition(const char *filename, Perso *perso);
void initCheckpointList(CheckpointList *list);
void addCheckpoint(CheckpointList *list, int position);
void checkCheckpoint(CheckpointList *list, Perso *perso, const char *filename);
void free_checkpoints(CheckpointList *list);

#endif
