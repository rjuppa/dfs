#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graph.h"


#define MAX_INPUT_LENGTH 256
#define MAX_NUMBER_LENGTH 16
#define MAX_DATA_LENGTH 36

char *dir_separator()
{
#if defined _WIN32 || defined __CYGWIN__
    return "\\";
#else
    return "/";
#endif
}


/* ____________________________________________________________________________

    Global Module Variables
   ____________________________________________________________________________
*/
char graph_fname[MAX_INPUT_LENGTH] = {0};       /* filename of a graph source file */
int start_id = 0;
int target_id = 0;
int max_distance = 5;


/* ____________________________________________________________________________

    void header()

    Prints a programme header to the console
   ____________________________________________________________________________
*/
void header() {
    printf("\n");
    printf("***** Search for Path using DFS ******\n");
    printf("* Seminar work of 'Programming in C' *\n");
    printf("*  Copyright (c) Radek Juppa, 2017   *\n");
    printf("**************************************\n\n");
}

/* ____________________________________________________________________________

    void help()

    Prints a short message how to use the program.
   ____________________________________________________________________________
*/
void help() {
    printf("Usage:\n   dfs.exe <filename> <id1> <id2> [maxD]\n\n");
    printf("Example:\n   dfs.exe graph.csv 1 2 4\n\n");
    printf("\n");
}


/* ____________________________________________________________________________

    int parseInteger(char *arg)

    Parses a numeric command-line argument and returns an integer.
   ____________________________________________________________________________
*/
int parseInteger(char *arg){
    int result = 0;
    char *input;
    char *ptr;
    input = (char *)malloc(1+ sizeof(char) * strlen(arg));
    strcpy(input, arg);
    result = (int) strtol
            (input, &ptr, 10);
    free((void *)input);

    if( result == 0){
        fprintf(stderr, "Error parsing a number: %s\n", arg);
        fprintf(stderr, "Use only numeric chars greater than zero. \n");
        exit(-1);
    }
    return result;
}


/* ____________________________________________________________________________

    void handle_input(int argc, char *argv[])

    Parses a command-line input to the program. If no arguments are
    given, prints out the help. Otherwise it searches for a path in given graph
    using DFS.
   ____________________________________________________________________________
*/
void handle_input(int argc, char *argv[]) {

    if (argc < 4) {     // show help
        header();
        help();
        exit(1);
    }

    strncpy(graph_fname, argv[1], MAX_INPUT_LENGTH - 1);
    graph_fname[MAX_INPUT_LENGTH - 1] = 0;

    start_id = parseInteger(argv[2]);
    target_id = parseInteger(argv[3]);
    if (argc > 4) {
        max_distance = parseInteger(argv[4]);
        max_distance++;
    }

    /*
    printf("graph_fname: %s\n", graph_fname);
    printf("start_id: %d\n", start_id);
    printf("target_node_id: %d\n", target_id);
    printf("max_distance: %d\n", max_distance);
     */
}

/* ____________________________________________________________________________

    char *parse_date(char *str)

    Parses date column from given string
   ____________________________________________________________________________
*/
char *parse_date(char *str){
    int i = 0;
    char c = 0;
    for(i=0; i<strlen(str); i++){
        c = str[i];
        if( c=='\r' || c=='\n' || c==0 ){
            break;
        }
    }
    str[i] = 0;
    return str;
}


/* ____________________________________________________________________________

    void parse_line(char *line, int *id1, int *id2, char *data)

    Parse a line of CSV text file. Semicolon is a separator.
   ____________________________________________________________________________
*/
void parse_line(char *line, int *id1, int *id2, char *data){
    char *pch;
    pch = strtok (line, ";");
    *id1 = (int) strtol(pch, (char **)NULL, 10);
    pch = strtok (NULL, ";");
    *id2 = (int) strtol(pch, (char **)NULL, 10);
    pch = strtok (NULL, ";");
    strcpy(data, parse_date(pch));
}


/* ____________________________________________________________________________

    void check_path(char *fname, char *path)

    Check weather given path does exist on disk
   ____________________________________________________________________________
*/
void check_path(char *fname, char *path){
    char cwd[1024];
    char *pch;

    pch = strstr(fname, dir_separator());
    if(pch == NULL) {
        getcwd(cwd, sizeof(cwd));
        strcpy(path, cwd);
        strcat (path, dir_separator());
        strcat (path, fname);
    }
    else{
        strcpy(path, fname);
    }

    if( access( path, F_OK ) == -1 ) {
        // file doesn't exist
        fprintf(stderr, "File doesn't exist. \n");
        fprintf(stderr, "Path: %s\n", path);
        exit(-1);
    }
}

/* ____________________________________________________________________________

    GRAPH *load_graph(char *path)

    This method opens a CSV file and iterates its content by line.
    Each line is loaded and new edge is added into the graph.
    It returns an pointer tp GRAPH
   ____________________________________________________________________________
*/
GRAPH *load_graph(char *path) {
    GRAPH *g;
    FILE *fr = NULL;                              /* graph file handle */
    char line[255] = {0};
    char data[MAX_DATA_LENGTH] = {0};
    int id1 = 0;
    int id2 = 0;

    g = graph_create();
    fr = fopen(path, "r");
    while(fgets(line, 255, fr) != NULL){
        parse_line(line, &id1, &id2, data);
        graph_add_edge(g, id1, id2, data);
    }
    fclose(fr);
    return g;
}



/* ____________________________________________________________________________

    MAIN PROGRAM
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
    GRAPH *g;
    VERTEX *v1;
    VERTEX *v2;
    char *path;

    path = (char *)malloc(sizeof(char) * 255);
    if(path == NULL){
        fprintf(stderr, "Out of memory.");
        exit(-1);
    }

    // parse command-line arguments
    handle_input(argc, argv);

    //strcpy(graph_fname, "/Users/radekj/devroot/C/dfs/sw-test.csv");
    check_path(graph_fname, path);

    g = load_graph(graph_fname);

    v1 = graph_get_vertex(g, start_id);
    if(v1 == NULL){
        fprintf(stderr, "Node id1=%d not found.\n", start_id);
        exit(-1);
    }

    v2 = graph_get_vertex(g, target_id);
    if(v2 == NULL){
        fprintf(stderr, "Node id2=%d not found.\n", target_id);
        exit(-1);
    }

    graph_search_for_path(g, start_id, target_id, max_distance);

    graph_show_results(g);

    graph_dispose(g);

    free((void *)path);

    return 0;
}
