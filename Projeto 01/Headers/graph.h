#ifndef _HEADER_GRAPH_H_
#define _HEADER_GRAPH_H_


typedef struct adjList_node;
typedef struct vertex;
typedef struct graph;


struct adjList { /* Node of an adjacency list */
    t_vertex * vertex; /* Pointer to adjacent vertex */
    struct adjList * next; /* Pointer to next node */
    struct adjList * prev;
} t_node;

struct vertex { /* Vertex structre */
    int length; /* Length of adjacency list attached to that vertex */
    void * content; /* Content stored in vetex */
    adjList * head; /* Pointer to adjacency list of that vertex */
} t_vertex;

struct graph { /* Graph structure */
    int size; /* Optional: easier to declare a graph with "size" vertexes */
    t_vertex * vertexArray; /* Array of vertexes on the graph */
} t_graph;


void error (char * msg) { /* Print error to output */
    printf("[Error:] %s\nEnding running file...\n", msg);
    exit(1)
}


#endif