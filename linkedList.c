#ifndef __LinkedList_C
#define __LinkedList_C

#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

Node *newNode( GraphNode *graphNode ) {
    Node *newNode = (Node *) malloc( sizeof( Node ) );
    newNode->node = graphNode;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void append( LinkedList *list, GraphNode *graphNode ) {
    //insert at the end
    Node *inNode = newNode( graphNode );
    Node *last = list->tail->prev;
    last->next = inNode;
    inNode->prev = last;
    inNode->next = list->tail;
    list->tail->prev = inNode;
}

LinkedList *newList() {
    LinkedList *linkedList = (LinkedList *) malloc( sizeof( LinkedList ) );
    linkedList->head = newNode( NULL );
    linkedList->tail = newNode( NULL );
    linkedList->head->next = linkedList->tail;
    linkedList->head->prev = NULL;
    linkedList->tail->prev = linkedList->head;
    linkedList->tail->next = NULL;
    return linkedList;
}

void deleteList( LinkedList *list ) {
    if ( !list ) return;
    Node *thisNode = list->head;
    while ( thisNode != NULL ) {
        Node *tmp = thisNode->next;
        free( thisNode );
        thisNode = tmp;
    }
    free( list );
}

GraphNode *newGraphNode( int value ) {
    GraphNode *newNode = (GraphNode *) malloc( sizeof( GraphNode ) );
    newNode->value = value;
    newNode->neighbors = newList();
    newNode->marked = 0;
    return newNode;
}

GraphNode *find( LinkedList *list, int value ) {
	Node *found = list->head->next;
	while ( found->node->value != value ) {
		found = found->next;
	}
	return found->node;
}

int hasNeighbor( GraphNode *node, int value ) {
    if ( node->value == value ) return 1;
    for ( Node *cur = node->neighbors->head->next; cur != node->neighbors->tail; cur = cur->next ) {
        if ( cur->node->value == value ) return 1;
    }
    return 0;
}

void link( GraphNode *node1, GraphNode *node2 ) {
    if ( node1 == node2 ) return;
    if ( !hasNeighbor( node1, node2->value ) && !hasNeighbor( node2, node1->value ) ) {
        append( node1->neighbors, node2 );
        append( node2->neighbors, node1 );
    }
}

void delete( GraphNode *graphNode ) {
    if ( graphNode->neighbors ) deleteList( graphNode->neighbors );
    free( graphNode );
}

#endif
