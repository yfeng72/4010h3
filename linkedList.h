#ifndef __LinkedList_H
#define __LinkedList_H

/**
 * Specifies the Linked List for edge lists, in the form of a doubly linked list
 */
struct graphNode_t;

typedef struct node_t {
    struct graphNode_t *node;
    struct node_t *prev;
    struct node_t *next;
} Node;

typedef struct linkedList_t {
    //head and tail are phantom nodes
    Node *head;
    Node *tail;
} LinkedList;

typedef struct graphNode_t {
    int value;
    LinkedList *neighbors;
} GraphNode;

/**
 * Appends a Node to the Linked List specified
 */
void append( LinkedList *list, GraphNode *graphNode );

/**
 * Returns whether the Linked List is empty (has no elements besides the phantom nodes)
 */
static inline int isEmpty( LinkedList *list ) { return list->head->next == list->tail ? 1 : 0; }

/**
 * Constructor for a new Linked List, allocates memory
 */
LinkedList *newList();

/**
 * Destructor for a Linked List, frees all the Entity Nodes and the list itself
 */
void deleteNode( GraphNode *node );

/**
 *
 * @param node : the current node
 * @param val : the neighbor node ID
 * @return boolean variable indicating whether the GraphNode has the specified neighbor
 */
int hasNeighbor( GraphNode *node, int val );

/**
 * Constructor for a new Node object, allocates memory
 */
Node *newNode( GraphNode *node );

/**
 *
 * @param value : the id for the graph node itself
 * @return : the linkedlist with head node as the graph node's ID
 */
GraphNode *newGraphNode( int value );

/**
 * Connects two graph nodes if they are not connected already
 * @param node1, node2 : graph nodes to be connected
 */
void link( GraphNode *node1, GraphNode *node2 );

void deleteList( LinkedList *list );

void delete( GraphNode *node );

#endif
