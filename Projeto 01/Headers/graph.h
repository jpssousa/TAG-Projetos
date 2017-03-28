#ifndef _HEADER_GRAPH_H_
#define _HEADER_GRAPH_H_


typedef struct adjList t_node;
typedef struct vertex t_vertex;
typedef struct graph t_graph;


struct adjList { /* Node of an adjacency list */
    t_vertex    * vertex; /* Pointer to adjacent vertex */
    t_node * next; /* Pointer to next node */
    t_node * prev;
};

struct vertex { /* Vertex structre */
    int length; /* Length of adjacency list attached to that vertex */
    void * content; /* Content stored in vetex */
    t_node * head; /* Pointer to adjacency list of that vertex */
};

struct graph { /* Graph structure */
    int size; /* Optional: easier to declare a graph with "size" vertexes */
    t_vertex * vertexArray; /* Array of vertexes on the graph */
};

/*void error (char * msg);*/

t_graph *createGraph    (int n);
void    destroyGraph    (t_graph * G);
void    addEdge         (t_graph * G, int src, int dest);

#endif
