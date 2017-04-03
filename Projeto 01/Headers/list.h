
/**
 *
 */

#ifndef _HEADER_LIST_
#define _HEADER_LIST_

typedef struct nodeL {
    void 			* data;
    struct nodeL 	* next;
    struct nodeL 	* prev;
} node_L;

node_L 	* createList 		(void * data);
void    destroyList 		(node_L * head);

void 	* pushToEnd 		(node_L * head, void * data);
node_L 	* pushToBegining	(node_L * head, void * data);

void 	* insertHere 		(node_L * head, void * data);
node_L 	* insertHere2 		(node_L * head, void * data);

#endif

