#ifndef __LinkedList_H
#define __LinkedList_H

/**
 * Specifies the Linked List for edge lists, in the form of a doubly linked list
 */

typedef struct node_t {
    int value;
    struct node_t *prev;
    struct node_t *next;
} Node;

typedef struct linkedList_t {
    //head and tail are phantom nodes
    Node *head;
    Node *tail;
} LinkedList;

/**
 * Appends a Node to the Linked List specified
 */
void append( LinkedList *list, int val );

/**
 * Returns whether the Linked List is empty (has no elements besides the phantom nodes)
 */
static inline int isEmpty( LinkedList *list ) { return list->head->next == list->tail ? 1 : 0; }

/**
 * Removes the last node
 */
int removeFirst( LinkedList *list );

/**
 * Constructor for a new Linked List, allocates memory
 */
LinkedList *newList();

/**
 * Destructor for a Linked List, frees all the Entity Nodes and the list itself
 */
void delete( LinkedList *list );

/**
 * Constructor for a new Node object, allocates memory
 */
Node *newNode( int value );

#endif
