//
// Created by Radek Juppa on 31/12/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

/* ____________________________________________________________________________

    GRAPH *graph_create()

    Creates instance of a graph structure and returns its reference
   ____________________________________________________________________________
*/
GRAPH *graph_create(){
    GRAPH *g;
    g = (GRAPH *)malloc(sizeof(GRAPH));
    if(g == NULL){
        printf("Out of memory.");
        exit(1);
    }
    g->first_vertex = NULL;
    g->count = 0;
    g->current_path = stack_create();
    g->result = NULL;
    return g;
}


/* ____________________________________________________________________________

    VERTEX *graph_get_vertex(GRAPH *g, int id)

    Returns a Vertex by its ID or NULL
   ____________________________________________________________________________
*/
VERTEX *graph_get_vertex(GRAPH *g, int id){
    VERTEX *current;
    if(g != NULL && g->first_vertex != NULL){
        current = g->first_vertex;
        while(current!=NULL){
            if(current->id == id){
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}


/* ____________________________________________________________________________

    VERTEX *graph_add_vertex(GRAPH *g, int id)

    Creates a new Vertex and returns it
   ____________________________________________________________________________
*/
VERTEX *graph_add_vertex(GRAPH *g, int id){
    VERTEX *v;
    VERTEX *current;
    v = graph_get_vertex(g, id);
    if( v == NULL ){
        // create a new vertex
        v = (VERTEX *)malloc(sizeof(VERTEX));
        if(v == NULL){
            printf("Out of memory.");
            exit(1);
        }
        v->id = id;
        v->first_edge = NULL;
        v->next = NULL;
        if( g->first_vertex == NULL ) {
            g->first_vertex = v;            // first time
        }
        else{
            current = g->first_vertex;
            while(current->next!=NULL){     // find the last item
                current = current->next;
            }
            current->next = v;              // add a new vertex on the end
        }
        g->count++;
    }
    return v;
}


/* ____________________________________________________________________________

    EDGE *graph_get_edge(GRAPH *g, int vertex_id, int id, char *data)

    Returns an existing Edge or NULL
   ____________________________________________________________________________
*/
EDGE *graph_get_edge(GRAPH *g, int vertex_id, int id, char *data) {
    VERTEX *v;
    EDGE *current;
    v = graph_get_vertex(g, vertex_id);
    if( v == NULL ) {
        return NULL;
    }

    if(v->first_edge != NULL){
        current = v->first_edge;
        while(current!=NULL){
            if(current->id == id && strcmp(current->data, data) == 0){
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}


/* ____________________________________________________________________________

    EDGE *graph_add_edge(GRAPH *g, int vertex_id, int id, char *data)

    Adds a new Edge into GRAPH and returns its reference
   ____________________________________________________________________________
*/
EDGE *graph_add_edge(GRAPH *g, int vertex_id, int id, char *data){
    EDGE *e;
    EDGE *current_edge;
    VERTEX *v;
    VERTEX *v2;

    v = graph_get_vertex(g, vertex_id);
    if( v == NULL ) {
        v = graph_add_vertex(g, vertex_id);
    }

    v2 = graph_get_vertex(g, id);
    if( v2 == NULL ) {
        graph_add_vertex(g, id);
    }

    e = graph_get_edge(g, vertex_id, id, data);
    if( e == NULL){
        // create a new edge
        e = (EDGE *)malloc(sizeof(EDGE));
        if(e == NULL){
            printf("Out of memory.");
            exit(1);
        }
        e->id = id;
        e->data = (char *)malloc(1 + sizeof(char) * strlen(data));
        strcpy(e->data, data);
        e->next = NULL;
        if( v->first_edge == NULL ){
            v->first_edge = e;                      // first time
        }
        else{
            current_edge = v->first_edge;
            while(current_edge->next != NULL){      // find the last item
                current_edge = current_edge->next;
            }
            current_edge->next = e;                 // add a new edge on the end
        }
    }
    return e;
}


/* ____________________________________________________________________________

    void graph_free_edge(EDGE *e)

    Dispose an Edge with its data
   ____________________________________________________________________________
*/
void graph_free_edge(EDGE *e){
    free((void *)e->data);
    e->data = NULL;
    free((void *)e);
    e = NULL;
}

/* ____________________________________________________________________________

    void graph_free_vertex(VERTEX *v)

    Dispose a Vertex with all underline edges
   ____________________________________________________________________________
*/
void graph_free_vertex(VERTEX *v){
    EDGE *first;
    EDGE *current;
    if(v->first_edge != NULL){
        first = v->first_edge;
        v->first_edge = NULL;
        current = first;
        while(current!=NULL){
            first = current->next;
            graph_free_edge(current);
            current = first;
        }
    }
    free((void *)v);
    v = NULL;
}


/* ____________________________________________________________________________

    void graph_dispose(GRAPH *g)

    Dispose graph with all underline structures
   ____________________________________________________________________________
*/
void graph_dispose(GRAPH *g){
    VERTEX *first;
    VERTEX *current;
    if(g != NULL && g->first_vertex != NULL){
        current = g->first_vertex;
        g->first_vertex = NULL;                     // free vertexes
        while(current!=NULL){
            first = current->next;
            graph_free_vertex(current);
            current = first;
        }
        g->count = 0;
        stack_dispose(g->current_path);
    }

    RESULT *result;
    RESULT *temp;                                   // free results
    if(g != NULL && g->result != NULL){
        result = g->result;
        while(result->next != NULL){
            temp = result->next;

            free(result->vertexes);
            result->vertexes = NULL;
            free(result->labels);
            result->labels = NULL;

            free(result);
            result = temp;
        }
        free(result->vertexes);
        free(result->labels);
        free(result);
    }

    free((void *)g);
    g = NULL;
}


/* ____________________________________________________________________________

    int days_from_2000(struct tm tm1)

    Counts days from 2000-01-01
   ____________________________________________________________________________
*/
int days_from_2000(struct tm tm1){
    struct tm tm_start;
    time_t t1, t2;
    double diff_t;

    memset(&tm_start, 0, sizeof(struct tm));
    strptime("2000-01-01 00:00:00", "%Y-%m-%d %H:%M:%S", &tm_start);

    t1 = mktime(&tm_start);
    t2 = mktime(&tm1);
    diff_t = difftime(t2, t1);
    int d = (int)(diff_t/60/60/24);
    return d;
}


/* ____________________________________________________________________________

    void sprint_date(struct tm tm_info, char *buffer)

    Prints a struct tm tm_info into given string
   ____________________________________________________________________________
*/
void sprint_date(struct tm tm_info, char *buffer){
    strftime(buffer, 11, "%Y-%m-%d", &tm_info);
}


/* ____________________________________________________________________________

    RESULT *graph_create_result(STACK *path)

    Creates a result and returns its reference
   ____________________________________________________________________________
*/
RESULT *graph_create_result(STACK *path){
    RESULT *r;
    int i = 0;
    int days = 0;
    int days_max = 0;
    int days_min = 0;

    r = (RESULT *)malloc(sizeof(RESULT));
    if(r == NULL){
        printf("Out of memory.");
        exit(1);
    }
    r->vertexes = stack_get_vertexes(path);
    r->labels = stack_get_labels(path);
    r->length = path->count-1;

    days_min = days_from_2000(r->labels[0]);
    days_max = days_min;
    for(i=0; i<path->count-1; i++){
        days = days_from_2000(r->labels[i]);
        if( days > days_max ){ days_max = days; }
        if( days < days_min ){ days_min = days; }
    }
    r->score = days_max - days_min;
    r->next = NULL;
    return r;
}


/* ____________________________________________________________________________

    void graph_add_result(GRAPH *g, RESULT *res)

    Adds a result into a list of results in graph.
    The list is ordered.
   ____________________________________________________________________________
*/
void graph_add_result(GRAPH *g, RESULT *res){
    RESULT *addr;
    RESULT *p_result;
    if( g->result == NULL ){
        g->result = res;                // insert the first result
        return;
    }

    p_result = g->result;
    if(res->length < p_result->length){
        res->next = p_result;           // insert before existing first
        g->result = res;
        return;
    }
    if((res->length == p_result->length && res->score < p_result->score)){
        res->next = p_result;           // insert before existing first
        g->result = res;
        return;
    }

    while(p_result != NULL){
        if( p_result->next != NULL ){
            if(res->length > p_result->next->length){
                p_result = p_result->next;
                continue;
            }
            if((res->length == p_result->next->length && res->score > p_result->next->score)){
                p_result = p_result->next;
                continue;
            }
            addr = p_result->next;     // insert it before the first larger
            p_result->next = res;
            res->next = addr;
        }
        else{
            p_result->next = res;      // insert it at the end
            res->next = NULL;
        }
        break;
    }
}


/* ____________________________________________________________________________

    void print_result(RESULT *res)

    Prints a single result
   ____________________________________________________________________________
*/
void print_result(RESULT *res){
    printf("%d", res->vertexes[res->length]);
    for(int i=res->length-1; i>=0; i--){
        printf("-%d", res->vertexes[i]);
    }
    char str_date[11] = {0};
    sprint_date(res->labels[res->length-1], str_date);
    printf(";%s", str_date);
    for(int i=res->length-2; i>=0; i--){
        sprint_date(res->labels[i], str_date);
        printf(",%s", str_date);
    }
    printf(";%d\n", res->score);
}


/* ____________________________________________________________________________

    void graph_show_results(GRAPH *g)

    Prints results to console
   ____________________________________________________________________________
*/
void graph_show_results(GRAPH *g){
    RESULT *res;
    res = g->result;
    if( res != NULL ){
        while(res != NULL){
            print_result(res);
            res = res->next;
        }
    }
    else{
        printf("No path found. (MaxDistance=%d)\n\n", g->limit);
    }
}


/* ____________________________________________________________________________

    void graph_dfs(GRAPH *g)

    This is recursive method traverses a given graph.
   ____________________________________________________________________________
*/
void graph_dfs(GRAPH *g){
    VERTEX *v;
    EDGE *edge;
    RESULT *res;

    int vid = stack_peek(g->current_path);
    v = graph_get_vertex(g, vid);
    edge = v->first_edge;
    while( edge != NULL ){
        if( g->current_path->count >= g->limit ){               // max distance reached
            break;
        }

        if( stack_contains(g->current_path, edge->id) == 1 ){
            edge = edge->next;
            continue;
        }

        if(edge->id == g->target_vertex_id){                    // target found
            stack_push(g->current_path, edge->id, edge->data);
            res = graph_create_result(g->current_path);
            graph_add_result(g, res);
            stack_pop(g->current_path);
            break;
        }

        stack_push(g->current_path, edge->id, edge->data);
        graph_dfs(g);
        stack_pop(g->current_path);
        edge = edge->next;
    }

}


/* ____________________________________________________________________________

    void graph_search_for_path(GRAPH *g, int start_vertex_id, int target_vertex_id, int limit)

    This method starts searching of paths in graph. It calls graph_dfs(GRAPH *g)
    to traverse given graph.
    param. start_vertex_id   - is starting vertex
    param. target_vertex_id  - is ending vertex
    param. limit             - is max distance between vertexes
    Results are stores in GRAPH struct.
   ____________________________________________________________________________
*/
void graph_search_for_path(GRAPH *g, int start_vertex_id, int target_vertex_id, int limit){
    g->start_vertex_id = start_vertex_id;           // set start_vertex_id
    g->target_vertex_id = target_vertex_id;         // set target_vertex_id
    g->limit = limit;

    stack_push(g->current_path, g->start_vertex_id, NULL);  // add the first vertex to path
    graph_dfs(g);                                   // start traversing the graph
}