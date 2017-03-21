#include <math.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct nodeL {
    int data;
    struct nodeL * next;
    struct nodeL * prev;
} node_L;


node_L * createList (int data) {
    node_L * list = (node_L *) malloc (sizeof (node_L));
    if (list) {
        list->data = data;
        list->next = NULL;
        list->prev = NULL;

        return node_L;
    }
    return NULL;
}


int pushToEnd (node_L * head, int data) {
    if (head == NULL) return 0;

    node_L * current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (node_L *) malloc (sizeof (node_L));
    current->next->prev = current;
    current->next->data = data;
    current->next->next = NULL;

    return 1;
}


int pushToBegining (node_L * head, int data) {
    if (head == NULL) return 0;

    node_L * current = head;

    while (current->prev != NULL) {
        current = current->prev;
    }

    current->prev = (node_L *) malloc (sizeof (node_L));
    current->prev->next = current;
    current->prev->data = data;
    current->prev->prev = NULL;

    return 1;
}


int insertHere (node_L * head, int data) {
    if (head == NULL) return 0;

    node_L * new = createList (data);
    node_L * aux = head->next;
    head->next = new;
    new->prev = head;
    aux->prev = aux;

    return 1;
}


int popThis (node_L * head) {
    if (head == NULL) return 0;

    int dt = head->data;
    if (head->prev != NULL) {
        head->prev->next = head->next;
    }
    if (head->next != NULL) {
        head->next->prev = head->prev;
    }
    free (head);

    return dt;
}


int popData (node_L * head, int data) {
    if (head == NULL) return 0;

    node_L * this = head;
    while (this->next != NULL && this->data != data) {
        this = this->next;
    }
    if (this->data == data) {
        return popThis (this);
    }
    return 0;
}


int getElement (node_L * head, int data) {
    if (head == NULL) return 0;

    node_L * this = head;
    while (this->next != NULL && this->data != data) {
        this = this->next;
    }
    if (this->data == data) {
        return this->data;
    }
    return 0;
}