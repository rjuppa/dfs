//
// Created by Radek Juppa on 01/01/2018.
//

#include "stack.h"
#include "parse_date.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


/* ____________________________________________________________________________

    STACK *stack_create()

    Creates a new instance of STACK and returns the reference
   ____________________________________________________________________________
*/
STACK *stack_create(){
    STACK *stack;
    stack = (STACK *)malloc(sizeof(STACK));
    if(stack == NULL){
        printf("Out of memory.");
        exit(1);
    }
    stack->top = NULL;
    stack->count = 0;
    return stack;
}


/* ____________________________________________________________________________

    void stack_print(STACK *stack)

    Helper method for printing the stack
   ____________________________________________________________________________
*/
void stack_print(STACK *stack){
    STACK_ITEM *item;
    item = stack->top;
    int arr[10];
    int n = 0;
    while(item != NULL){
        arr[n++] = item->id;
        item = item->next;
    }
    for(int i=n-1; i>=0; i--){
        printf(" %d ", arr[i]);
    }
    printf("\n");
}


/* ____________________________________________________________________________

    void stack_push(STACK *stack, int id, char *data)

    Adds a value to the top of stack
   ____________________________________________________________________________
*/
void stack_push(STACK *stack, int id, char *data){
    STACK_ITEM *item;
    item = (STACK_ITEM *)malloc(sizeof(STACK_ITEM));
    if(item == NULL){
        printf("Out of memory.");
        exit(1);
    }
    item->id = id;
    item->data = data;
    item->next = stack->top;
    stack->top = item;
    stack->count++;
}


/* ____________________________________________________________________________

    int stack_pop(STACK *stack)

    Removes and returns the top value from the top of stack
   ____________________________________________________________________________
*/
int stack_pop(STACK *stack){
    STACK_ITEM *item;
    item = stack->top;
    if(stack->top == NULL){
        fprintf(stderr, "Calling 'stack_pop' when stack is empty. \n");
        exit(-1);
    }
    stack->top = item->next;
    int id = item->id;
    free(item);
    stack->count--;
    return id;
}


/* ____________________________________________________________________________

    int stack_peek(STACK *stack)

    Reads the top value(vertexId) from the stack
   ____________________________________________________________________________
*/
int stack_peek(STACK *stack){
    if(stack->top == NULL){
        fprintf(stderr, "Calling 'stack_peek' when stack is empty. \n");
        exit(-1);
    }
    return stack->top->id;
}


/* ____________________________________________________________________________

    void stack_dispose(STACK *stack)

    Frees memory of all items in the stack
   ____________________________________________________________________________
*/
void stack_dispose(STACK *stack){
    STACK_ITEM *item;
    STACK_ITEM *top;
    item = stack->top;
    while(item != NULL){
        top = item->next;
        free(item);
        stack->count--;
        item = top;
    }
    free(stack);
    stack = NULL;
}


/* ____________________________________________________________________________

    int stack_contains(STACK *stack, int id)

    Check if the value is in the stack
   ____________________________________________________________________________
*/
int stack_contains(STACK *stack, int id){
    STACK_ITEM *item;
    item = stack->top;
    while(item != NULL){
        if(item->id == id){
            return 1;
        }
        item = item->next;
    }
    return 0;
}


/* ____________________________________________________________________________

    int *stack_get_vertexes(STACK *path)

    Returns an array of values(ids) in current stack
   ____________________________________________________________________________
*/
int *stack_get_vertexes(STACK *path){
    STACK_ITEM *item;
    int *vertexes;
    vertexes = (int *)malloc(sizeof(int) * path->count);
    if(vertexes == NULL){
        printf("Out of memory.");
        exit(-1);
    }
    item = path->top;
    int i = 0;
    while(item != NULL){
        vertexes[i++] = item->id;
        item = item->next;
    }
    return vertexes;
}


/* ____________________________________________________________________________

    struct tm *stack_get_labels(STACK *path)

    Returns an array of values(labels) in current stack
   ____________________________________________________________________________
*/
struct tm *stack_get_labels(STACK *path){
    STACK_ITEM *item;
    struct tm tm1;
    struct tm *labels;
    labels = (struct tm *)malloc(sizeof(struct tm) * (path->count-1));
    if(labels == NULL){
        printf("Out of memory.");
        exit(1);
    }
    item = path->top;
    int i = 0;
    while(item != NULL){
        if( item->data !=NULL ){
            memset(&tm1, 0, sizeof(struct tm));
            strparsetime("2000-01-01", &tm1);
            if(strparsetime(item->data, &tm1) != 0){
                fprintf(stderr, "\nstrptime failed.\n");
            }
            labels[i++] = tm1;
        }
        item = item->next;
    }
    return labels;
}