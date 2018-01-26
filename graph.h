//
// Created by Radek Juppa on 31/12/2017.
//

#ifndef DFS_GRAPH_H
#define DFS_GRAPH_H

#include <time.h>
#include "stack.h"

typedef struct result{                  // RESULT (as sorted linked list)
    int *vertexes;                      // array of vertex ids
    struct tm *labels;                  // array of labels (datetime struct)
    int length;                         // length of path
    int score;                          // number of days between the first and the last date
    struct result *next;                // next item
} RESULT;


typedef struct edge{                    // EDGE (as linked list)
    int id;                             // id of vertex creating an edge with its parent VERTEX
    char *data;                         // data(label) assign to the edge
    struct edge *next;                  // next item
} EDGE;


typedef struct vertex{                  // VERTEX (as linked list)
    int id;                             // id of the vertex
    EDGE *first_edge;                   // reference to the first edge (as linked list)
    struct vertex *next;                // next item
} VERTEX;


typedef struct graph{
    VERTEX *first_vertex;               // reference to the first vertex(as linked list)
    STACK *current_path;                // current path as a stack of vertexes
    int count;                          // count of vertexes in the graph
    int start_vertex_id;                // starting vertex for search
    int target_vertex_id;               // ending vertex for search
    int limit;                          // max distance to search
    RESULT *result;                     // reference to results
} GRAPH;


GRAPH *graph_create();
void graph_add_edge(GRAPH *g, int vertex_id, int id, char *data);
VERTEX *graph_get_vertex(GRAPH *g, int vertex_id);
void graph_dispose(GRAPH *g);
void graph_search_for_path(GRAPH *g, int start_vertex_id, int target_vertex_id, int limit);
RESULT *graph_create_result(STACK *path);
void graph_add_result(GRAPH *g, RESULT *res);
void graph_show_results(GRAPH *g);

#endif //DFS_GRAPH_H
