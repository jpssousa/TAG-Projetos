#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void error (char * msg) { /* Print error to output */
    printf("[Error:] %s\nEnding running file...\n", msg);
    exit(1);
}

t_node * findEndOfList (t_node * l) {
    if (l == NULL) return NULL;
    while (l->next != NULL) l = l->next;
    return l;
}


t_node * findElementOnList (t_node * l, t_vertex * v) {
    if (l == NULL || v == NULL) return NULL;
    while (l->vertex != v) l = l->next;
    return l;
}


void removeElement (t_node * l, t_vertex * v) {
    if (l != NULL && v != NULL) {
        t_node * aux = findElementOnList (l, v);
        if (aux) {
            if (aux->next == NULL) {
                free (aux);
            }
            else {
                t_node * tmp = aux;
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                free (tmp);
            }
        }
    }
}


t_graph * createGraph (int n) {
    int i;

    t_graph * G = (t_graph *) malloc (sizeof (t_graph));
    if (G == NULL) {
        error ("Unable to allocate memory for graph.");
        return NULL;
    }
    G->size = n;
    G->vertexArray = NULL;

    if (n > 0) {
        G->vertexArray = (t_vertex *) malloc (n * sizeof (t_vertex));
        if (G->vertexArray == NULL){
            free(G);
            error ("Unable to allocate memory for graph's vertices.");
            return NULL;
        }
        for (i = 0; i < n; i++) {
            G->vertexArray[i].head = NULL;
            G->vertexArray[i].length = 0;
        }
    }

    return G;
}


void destroyGraph (t_graph * G) {
    if (G) {
        if (G->vertexArray) {
            int i;

            for (i = 0; i < G->size; i++) {
                t_node * adjList = G->vertexArray[i].head;
                while (adjList) { /* Free adjcency list */
                    t_node * aux = adjList;
                    adjList = adjList->next;
                    free (aux);
                }
            }

            free (G->vertexArray); /* Free vertexes */
        }

        free (G); /* Free graph */
    }
}



void createVertexG (t_graph * G, void * content) {
    if (G == NULL) {
        error ("Cannot to add vertex to null graph.");
        return;
    }

    t_vertex * newVertex = (t_vertex *) malloc (sizeof (t_vertex));
    if (newVertex == NULL) {
        error ("Unable to allocate memory for vertex.");
        return;
    }
    newVertex->content = content;
    newVertex->head = NULL;
    /*G->vertexArray[G->size] = newVertex;*/
    G->size++;
}


t_vertex * createVertex (void * content) {
    t_vertex * newVertex = (t_vertex *) malloc (sizeof (t_vertex));
    if (newVertex == NULL) {
        error ("Unable to allocate memory for vertex.");
        return NULL;
    }
    newVertex->content = content;
    newVertex->head = NULL;

    return newVertex;
}


void addEdge (t_graph * G, int src, int dest) {
    if (G == NULL) {
        error ("Impossible to add edge to null graph.");
        return;
    }

    if (src >= G->size || dest >= G->size) {
        error ("Impossible to add edge to null vertexes.");
        return;
    }

    t_node * l = findEndOfList (G->vertexArray[src].head);
    t_node * m = findEndOfList (G->vertexArray[dest].head);

    /*@asm95: sad code below*/
    if(!l){
        t_node *newNode = (t_node *) malloc (sizeof (t_node));
        newNode->vertex = &G->vertexArray[src];
        newNode->next = NULL;
        newNode->prev = NULL;
        G->vertexArray[src].head = newNode;
    } else {
        l->next = (t_node *) malloc (sizeof (t_node));
        l->next->vertex = &G->vertexArray[src];
        l->next->prev = l;
        l->next->next = NULL;
    }
    (G->vertexArray[src].length)++;

    if(!m){
        t_node *newNode = (t_node *) malloc (sizeof (t_node));
        newNode->vertex = &G->vertexArray[src];
        newNode->next = NULL;
        newNode->prev = NULL;
        G->vertexArray[dest].head = newNode;
    } else {
        m->next = (t_node *) malloc (sizeof (t_node));
        m->next->vertex = &G->vertexArray[dest];
        m->next->prev = m;
        m->next->next = NULL;
    }

}

/*
void removeEdge (t_graph * G, int src, int dest) {
    if (G == NULL) {
        error ("Impossible to remove edge to null graph.");
    }

    if (src >= G->size || dest >= G->size) {
        error ("Impossible to remove edge to null vertexes.");
    }


}
*/

