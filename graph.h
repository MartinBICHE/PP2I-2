#ifndef GRAPH_H
#define GRAPH_H
#include <stdlib.h> 
#include <stdio.h>
#include <stdbool.h>
#include "map.h"
#define MAX_NODES 3500

struct _Node{
    int x, y;
    bool walkable;
    struct _Node **neighbors;
    int num_neighbors;
    int cost;
    int g_cost;
    int h_cost;
    struct _Node *parent;
};
typedef struct _Node Node;



Node **create_graph(Map *map);
void print_graph(Node **graph, Map *map);

int manhattan_distance(Node *a, Node *b);
Node *find_lowest_cost_node(Node **open_list, int open_list_size);
bool node_in_list(Node **list, int list_size, Node *node);
void print_node(Node *node);
void find_and_print_path(Node **graph, Map *map, Node *start, Node *end);
void print_list(int *list, int length);
Node *a_star(Node **graph, Map *map, Node *start_node, Node *goal_node);
int *create_array(int n);
int len_nodes(Node array[MAX_NODES]);

void invert_array(Node arr[], int n);
Node* invert_node_array(const Node *array, int length);
void free_graph(Node **graph, Map *map);
Node* get_nearest_walkable_neighbor(Node** graph, Map *map, int xPos, int yPos);
void destroy_graph(Node **graph, Map *map);

#endif
