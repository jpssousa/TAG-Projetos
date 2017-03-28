#include <stdio.h>
#include <stdlib.h>
#include "list.h"


node_L * createList (void * data) {
    node_L * list = (node_L *) malloc (sizeof (node_L));
    if (list) {
        list->data = data;
        list->next = NULL;
        list->prev = NULL;
    }

    return list;
}

void destroyList (node_L * head){
    node_L * cur, * next;

    cur = head;
    while(cur){
        next = cur->next;
        free(cur);
        cur = next;
    }
}


void * pushToEnd (node_L * head, void * data) {
    if (head == NULL) return NULL;

    node_L * current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (node_L *) malloc (sizeof (node_L));
    current->next->prev = current;
    current->next->data = data;
    current->next->next = NULL;

    return data;
}


node_L * pushToBegining (node_L * head, void * data) {
    if (head == NULL) return NULL;

    node_L * current = head;

    while (current->prev != NULL) {
        current = current->prev;
    }

    current->prev = (node_L *) malloc (sizeof (node_L));
    current->prev->next = current;
    current->prev->data = data;
    current->prev->prev = NULL;

    return current->prev;
}


void * insertHere (node_L * head, void * data) {
    if (head == NULL) return NULL;

    node_L * new = createList (data);
    node_L * aux = head->next;
    head->next = new;
    new->prev = head;
    new->next = aux;

    if (aux)
        aux->prev = new;

    return data;
}


void * popThis (node_L * head) {
    if (head == NULL) return NULL;

    void * dt = head->data;
    if (head->prev != NULL) {
        head->prev->next = head->next;
    }
    if (head->next != NULL) {
        head->next->prev = head->prev;
    }
    free (head);

    return dt;
}


void * popData (node_L * head, void * data, int (*compareFunction)(void*, void*)) {
    if (head == NULL) return 0;

    node_L * this = head;
    while (this->next != NULL && this->data != data) {
        this = this->next;
    }
    if (compareFunction(this->data, data)){
        return popThis (this);
    }
    return NULL;
}


void * getElement (node_L * head, void * data, int (*compareFunction)(void*, void*)) {
    if (head == NULL) return 0;

    node_L * this = head;
    while (this->next != NULL && this->data != data) {
        this = this->next;
    }
    if (compareFunction(this->data, data)){
        return this->data;
    }
    return NULL;
}

