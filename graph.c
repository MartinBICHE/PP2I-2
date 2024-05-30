#include <stdlib.h> 
#include <stdio.h>
#include <stdbool.h>
#include "textures.h"
#include "enemyBat.h"
#include "graph.h"


Node **create_graph(Map *map){
    Node **graph = (Node **)malloc(map->height * sizeof(Node*));
    for (int i = 0; i < map->height; i++){
        graph[i] = (Node *)malloc(map->width * sizeof(Node));
    }
    for (int i = 0; i < map->height; i++){
        for (int j = 0; j < map->width; j++){
            Node *node = &graph[i][j];
            node->x = j;
            node->y = i;
            node->walkable = map->matrix[i][j] == '-';
            node->neighbors = NULL;
            node->num_neighbors = 0;
            node->cost = 1;
            node->g_cost = 0;
            node->h_cost = 0;
            node->parent = NULL;
        }
    }
    for (int i = 0; i < map->height; i++){
        for (int j = 0; j < map->width; j++){
            Node *node = &graph[i][j];
            if (node->walkable){
                int dxdy[8][2] = {{0, 1}, {1,0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
                for (int k = 0; k < 8; k++){
                    int nx = j + dxdy[k][0];
                    int ny = i + dxdy[k][1];

                    if (nx >= 0 && nx < map->width && ny >= 0 && ny < map->height && graph[ny][nx].walkable){
                        node->neighbors = (Node **)realloc(node->neighbors, (node->num_neighbors + 1) * sizeof(Node *));
                        node->neighbors[node->num_neighbors] = &graph[ny][nx];
                        node->num_neighbors++;
                    }
                }
            }
        }
    }
    return graph;
}
/* donne des memory leaks cette chose, à règler après */
void free_graph(Node **graph, Map *map) {
    if (graph == NULL) {
        return;
    }
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (graph[i][j].neighbors != NULL){
                free(graph[i][j].neighbors);
            }
        }
        free(graph[i]);  // Free each row
    }
    free(graph);  // Free the graph itself
}


void print_graph(Node **graph, Map *map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            Node *node = &graph[i][j];
            if (node->walkable) {
                printf("Node (%d, %d) has %d neighbors:\n", node->x, node->y, node->num_neighbors);
                for (int k = 0; k < node->num_neighbors; k++) {
                    printf("  (%d, %d)\n", node->neighbors[k]->x, node->neighbors[k]->y);
                }
            }
        }
    }
}

int manhattan_distance(Node *a, Node *b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

int euclidean_distance(Node *a, Node *b) {
    return round(sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)));
}

Node *find_lowest_cost_node(Node **open_list, int open_list_size) {
    int lowest_cost_index = 0;
    for (int i = 1; i < open_list_size; i++) {
        if (open_list[i]->g_cost + open_list[i]->h_cost < open_list[lowest_cost_index]->g_cost + open_list[lowest_cost_index]->h_cost) {
            lowest_cost_index = i;
        }
    }
    return open_list[lowest_cost_index];
}

bool node_in_list(Node **list, int list_size, Node *node) {
    for (int i = 0; i < list_size; i++) {
        if (list[i] == node) {
            return true;
        }
    }
    return false;
}



void print_node(Node *node) {
    if (node == NULL) {
        printf("Node is NULL\n");
        return;
    }
    printf("Node Information:\n");
    printf("  Position: (%d, %d)\n", node->x, node->y);
    if (node->walkable){
        printf(" Walkable: yes\n");
    }else{
        printf(" Walkable: no\n");
    }
    printf("  Num Neighbors: %d\n", node->num_neighbors);
    printf("  Cost: %d\n", node->cost);
    printf("  G Cost: %d\n", node->g_cost);
    printf("  H Cost: %d\n", node->h_cost);
    if (node->parent != NULL) {
        printf("  Parent: (%d, %d)\n", node->parent->x, node->parent->y);
    } else {
        printf("  Parent: NULL\n");
    }
}
void print_list(int *list, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
}


int len_nodes(Node array[MAX_NODES]){
    int i = 0;
    int len = 0;
    while (array[i].parent != NULL && i < MAX_NODES){
        len ++;
        i ++;
    }
    return len;
}

Node *a_star(Node **graph, Map *map, Node *start_node, Node *goal_node) {
    if (!goal_node ->walkable ){
        puts("Error: the goal is not walkable");
        print_node(goal_node);
        exit(-1);
    }
    if (!start_node ->walkable ){
        puts("Error:  start is not walkable");
        print_node(start_node);
        exit(-1);
    }
    int initialCapacity = 1500;
    int open_list_size = 0;
    int closed_list_size = 0;
    Node *array = calloc(MAX_NODES, sizeof(Node));
    Node **open_list = (Node **)malloc(MAX_NODES * sizeof(Node *));
    Node **closed_list = (Node **)malloc(MAX_NODES* sizeof(Node *));

    start_node->g_cost = 0;
    start_node->h_cost = euclidean_distance(start_node, goal_node);
    open_list[open_list_size++] = start_node;
    int path_size = 0;
    while (open_list_size > 0) {
        Node *current_node = find_lowest_cost_node(open_list, open_list_size);
        if (current_node->x == goal_node->x && current_node->y == goal_node->y) {
            while (current_node != start_node) {
                array[path_size++] = *current_node;
                current_node = current_node->parent;

            }
            free(open_list);
            free(closed_list);
            return array;
        }
             /* printf("\n"); */
        /* printf("g_cost: %d, h_cost: %d, x: %d, y: %d\n", current_node->g_cost, current_node->h_cost, current_node->x, current_node->y); */

        for (int i = 0; i < current_node->num_neighbors; i++) {
            Node *neighbor = current_node->neighbors[i];
            int tentative_g_cost = current_node->g_cost + current_node->cost;

            if (!node_in_list(closed_list, closed_list_size, neighbor) && (tentative_g_cost < neighbor->g_cost || !node_in_list(open_list, open_list_size, neighbor)))
            {
                neighbor->parent = current_node;
                neighbor->g_cost = tentative_g_cost;
                neighbor->h_cost = euclidean_distance(neighbor, goal_node);

                if (!node_in_list(open_list, open_list_size, neighbor)) {
                    open_list[open_list_size++] = neighbor;
                }
            }
        }

        closed_list[closed_list_size++] = current_node;
        for (int i = 0; i < open_list_size; i++) {
            if (open_list[i] == current_node) {
                for (int j = i; j < open_list_size - 1; j++) {
                    open_list[j] = open_list[j + 1];
                }
                open_list_size--;
                break;
            }
        }
    }

    free(open_list);
    free(closed_list);
    return NULL;
}

int directions[4][2] = {
    {-1, 0}, // up
    {1, 0},  // down
    {0, -1}, // left
    {0, 1}   // right
};

Node* get_nearest_walkable_neighbor(Node** graph, Map *map, int xPos, int yPos) {
    for (int i = 0; i < 4; ++i) {
        int newX = xPos + directions[i][1];
        int newY = yPos + directions[i][0];
        
        if (newX >= 0 && newX < map->width && newY >= 0 && newY < map->height) {
            Node* neighbor = &graph[newY][newX];
            if (neighbor->walkable) {
                return neighbor;
            }
        }
    }
    return NULL;
}

void destroy_graph(Node **graph, Map *map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            Node *node = &graph[i][j];
            if (node->neighbors) {
                free(node->neighbors);
                node->neighbors = NULL;
            }
        }
        free(graph[i]);
    }
    free(graph);
}

