//
// Created by Radek Juppa on 01/01/2018.
//

#ifndef DFS_STACK_H
#define DFS_STACK_H


typedef struct stack_item{
    int id;                         // id of a vertex
    char *data;                     // label of an edge
    struct stack_item *next;        // reference to next item in linked list
} STACK_ITEM;


typedef struct stack{
    STACK_ITEM *top;                // the first vertex
    int count;                      // count of items in the stack
} STACK;


STACK *stack_create();
void stack_push(STACK *stack, int id, char *data);
int stack_pop(STACK *stack);
int stack_peek(STACK *stack);
void stack_dispose(STACK *stack);
int stack_contains(STACK *stack, int id);
int *stack_get_vertexes(STACK *path);
struct tm *stack_get_labels(STACK *path);

#endif //DFS_STACK_H
